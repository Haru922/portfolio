class Solution:
    def read(self, buf, n):
        buf4 = ['']*4
        answer = 0
        
        for i in range(n//4):
            chars = read4(buf4)
            for j in range(chars):
                buf[answer] = buf4[j] 
                answer += 1
                
        for i in range(min(n%4,read4(buf4))):
            buf[answer] = buf4[i]
            answer += 1
            
        return answer
