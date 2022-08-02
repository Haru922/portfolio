class Logger:

    def __init__(self):
        self.log_timestamps = {}

    def shouldPrintMessage(self, timestamp: int, message: str) -> bool:
        log_timestamp = self.log_timestamps.get(message, -1)
        if (log_timestamp == -1) or (log_timestamp+10 <= timestamp):
            self.log_timestamps[message] = timestamp
            return True
        return False
