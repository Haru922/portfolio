package main

import (
	"bytes"
	"context"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"os"
	"os/signal"
	"strconv"
	"strings"
	"sync"
	"syscall"
	"time"
)

var counter = struct {
	sync.RWMutex
	visited map[string]int
}{visited: make(map[string]int)}

var expireTime = struct {
	year  int
	month time.Month
	day   int
}{}

var logWriter = struct {
	logger *log.Logger
	fp     *os.File
}{}

var config = struct {
	FeedbackServerPort     int    `json:"feedbackServerPort"`
	FeedbackServerLog      string `json:"feedbackServerLog"`
	ClientRequestThreshold int    `json:"clientRequestThreshold"`
	BugTrackingServer      string `json:"bugTrackingServer"`
	ApiToken               string `json:"apiToken"`
}{}

type Feedback struct {
	Title       string `json:"title"`
	Category    string `json:"category"`
	Release     string `json:"release"`
	Codename    string `json:"codename"`
	Description string `json:"description"`
}

func setCounterNow() {
	counter.Lock()
	defer counter.Unlock()
	counter.visited = make(map[string]int)
}

func validateFeedback(r *http.Request) bool {
	checkExpireTime()

	counter.RLock()
	n := counter.visited[r.RemoteAddr]
	counter.RUnlock()

	if n >= config.ClientRequestThreshold {
		return false
	}

	counter.Lock()
	defer counter.Unlock()
	counter.visited[r.RemoteAddr]++

	return true
}

func checkExpireTime() {
	year, month, day := time.Now().Date()

	if expireTime.year != year ||
		expireTime.month != month ||
		expireTime.day != day {
		gfbInit()
	}
}

func makeRequest(fb *Feedback) (*http.Request, error) {
	issue := fmt.Sprintf(`{"summary": "%s", "description": "%s", "category": {"name": "%s"}, "project": {"name": "%s"}}`,
		fb.Title, fb.Description, fb.Category, "Gooroom Feedback")

	req, err := http.NewRequest("POST", config.BugTrackingServer+"/mantis/api/rest/issues/", bytes.NewBuffer(bytes.NewBufferString(issue).Bytes()))
	if err != nil {
		return nil, err
	}

	defer req.Body.Close()
	req.Header.Add("Authorization", config.ApiToken)
	req.Header.Add("Content-Type", "application/json")

	return req, nil
}

func getFeedback(r *http.Request) (*Feedback, error) {
	var title, category, release, codename, description string

	if err := r.ParseForm(); err != nil {
		return nil, err
	}

	for k, v := range r.Form {
		switch strings.ToLower(k) {
		case "title":
			title = v[0]
		case "category":
			category = v[0]
		case "release":
			release = v[0]
		case "codename":
			codename = v[0]
		case "description":
			description = strings.ReplaceAll(v[0], "\n", "\\n")
		}
	}

	return &Feedback{Title: title, Category: category, Release: release, Codename: codename, Description: description}, nil
}

func statusHandler(w http.ResponseWriter, r *http.Request) {
	logWriter.logger.Printf("[Gooroom-Feedback-Server] => Request Received. (Status, Sender: %s)\n", r.RemoteAddr)

	if r.Method == "GET" {
		w.WriteHeader(http.StatusOK)
		w.Write([]byte(http.StatusText(http.StatusOK)))

		logWriter.logger.Println("[Gooroom-Feedback-Server] => Running...")
	} else {
		w.WriteHeader(http.StatusMethodNotAllowed)
		w.Write([]byte(http.StatusText(http.StatusMethodNotAllowed)))

		logWriter.logger.Println("[Gooroom-Feedback-Server] => Not Allowed Method.")
	}
}

func feedbackHandler(w http.ResponseWriter, r *http.Request) {
	logWriter.logger.Printf("[Gooroom-Feedback-Server] => Request Received. (Feedback, Sender: %s)\n", r.RemoteAddr)

	if r.Method == "POST" {
		if validateFeedback(r) {
			logWriter.logger.Println("[Gooroom-Feedback-Server] => Request Validated.")
			logWriter.logger.Println("=========================== [FEEDBACK] ===========================")
			logWriter.logger.Printf("Method: %s, URL: %s, Proto: %s\n", r.Method, r.URL, r.Proto)
			for k, v := range r.Header {
				logWriter.logger.Printf("%q = %q\n", k, v)
			}

			fb, err := getFeedback(r)
			if err != nil {
				logWriter.logger.Printf("[Gooroom-Feedback-Server] => %s\n", err)

				w.WriteHeader(http.StatusBadRequest)
				w.Write([]byte(http.StatusText(http.StatusBadRequest)))

				return
			}

			logWriter.logger.Printf("%+v\n", fb)
			logWriter.logger.Println("==================================================================")

			req, err := makeRequest(fb)
			if err != nil {
				logWriter.logger.Printf("[Gooroom-Feedback-Server] => %s\n", err)

				w.WriteHeader(http.StatusInternalServerError)
				w.Write([]byte(http.StatusText(http.StatusInternalServerError)))

				return
			}

			logWriter.logger.Println("[Gooroom-Feedback-Server] => Issue Created")

			client := &http.Client{Timeout: 3 * time.Second}
			resp, err := client.Do(req)
			if err != nil {
				w.WriteHeader(http.StatusInternalServerError)
				w.Write([]byte(err.Error()))

				logWriter.logger.Println("[Gooroom-Feedback-Server] => Internal Server Error.")
				logWriter.logger.Printf("ERROR: %s\n", err)

				return
			}
			defer resp.Body.Close()

			logWriter.logger.Println("[Gooroom-Feedback-Server] => Issue Requested.")

			logWriter.logger.Println(resp)

			w.WriteHeader(resp.StatusCode)
			w.Write([]byte(http.StatusText(resp.StatusCode)))
		} else {
			w.WriteHeader(http.StatusNotAcceptable)
			w.Write([]byte(http.StatusText(http.StatusNotAcceptable)))

			logWriter.logger.Println("[Gooroom-Feedback-Server] => Request Invalidated.")
		}
	} else {
		w.WriteHeader(http.StatusMethodNotAllowed)
		w.Write([]byte(http.StatusText(http.StatusMethodNotAllowed)))

		logWriter.logger.Println("[Gooroom-Feedback-Server] => Not Allowed Method.")
	}
}

func setExpireTimeNow() {
	expireTime.year, expireTime.month, expireTime.day = time.Now().Date()
}

func setLogWriterNow() {
	if logWriter.fp != nil {
		if err := logWriter.fp.Close(); err != nil {
			panic(err)
		}
	}

	logFile := fmt.Sprintf("/var/log/gfb-%d-%02d-%02d.log", expireTime.year, expireTime.month, expireTime.day)

	fp, err := os.OpenFile(logFile, os.O_CREATE|os.O_WRONLY|os.O_APPEND, 0644)
	if err != nil {
		panic(err)
	}

	logWriter.fp = fp
	logWriter.logger = log.New(logWriter.fp, "", log.LstdFlags|log.Lshortfile)
	logWriter.logger.Printf("[Gooroom-Feedback-Server] => Date: %d-%02d-%02d\n",
		expireTime.year, expireTime.month, expireTime.day)
}

func gfbInit() {
	setExpireTimeNow()

	setCounterNow()

	setLogWriterNow()

	logWriter.logger.Println("[Gooroom-Feedback-Server] => Initialized.")
}

func gfbLoadConfig() bool {
	configJson, err := os.Open("/etc/gooroom/gooroom-feedback-server/gooroom-feedback-server.conf")
	if err != nil {
		logWriter.logger.Fatalf("[Gooroom-Feedback-Server] => %v", err)
		return false
	}
	defer configJson.Close()

	contents, _ := ioutil.ReadAll(configJson)

	json.Unmarshal(contents, &config)

	logWriter.logger.Println("[Gooroom-Feedback-Server] => Feedback Server Port: " + strconv.Itoa(config.FeedbackServerPort))
	logWriter.logger.Println("[Gooroom-Feedback-Server] => Client Request Threshold: " + strconv.Itoa(config.ClientRequestThreshold))
	logWriter.logger.Println("[Gooroom-Feedback-Server] => Bug Tracking Server: " + config.BugTrackingServer)

	return true
}

func main() {
	gfbInit()

	if gfbLoadConfig() {
		srv := &http.Server{
			Addr:         ":" + strconv.Itoa(config.FeedbackServerPort),
			WriteTimeout: 3 * time.Second,
			ReadTimeout:  3 * time.Second,
			Handler:      nil,
		}

		http.HandleFunc("/gooroom/feedback/new", feedbackHandler)
		http.HandleFunc("/status", statusHandler)

		idleConnsClosed := make(chan struct{})

		go func() {
			done := make(chan os.Signal, 1)
			signal.Notify(done, os.Interrupt, syscall.SIGINT, syscall.SIGTERM)

			<-done

			if err := srv.Shutdown(context.Background()); err != nil {
				logWriter.logger.Printf("[Gooroom-Feedback-Server] => %v", err)
			}

			close(idleConnsClosed)
		}()

		logWriter.logger.Println("[Gooroom-Feedback-Server] => Serving...")

		if err := srv.ListenAndServe(); err != nil {
			logWriter.logger.Fatalf("[Gooroom-Feedback-Server] => %v", err)
		}

		<-idleConnsClosed
	} else {
		logWriter.logger.Fatalf("[Gooroom-Feedback-Server] => Configuration Error.")
	}
}
