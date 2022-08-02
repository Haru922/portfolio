class MRUQueue:
    
    def __init__(self, n: int):
        self.queue = [num+1 for num in range(n)]

    def fetch(self, k: int) -> int:
        num = self.queue.pop(k-1)
        self.queue.append(num)
        return num
