class Solution:
    def minOperations(self, logs: List[str]) -> int:
        folders = 0 
        for log in logs:
            if log == '../':
                if folders:
                    folders -= 1
            elif log != './':
                folders += 1
        return folders
