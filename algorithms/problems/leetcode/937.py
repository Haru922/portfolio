class Solution:
    def reorderLogFiles(self, logs: List[str]) -> List[str]:
        def key_function(log):
            identifier, message = log.split(maxsplit=1)
            return (0, message, identifier) if message[0].isalpha() else (1,)
        return sorted(logs, key=key_function)
