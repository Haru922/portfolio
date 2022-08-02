class CustomStack:
    def __init__(self, maxSize: int):
        self.stack=[]
        self.max_size=maxSize

    def push(self, x: int) -> None:
        if len(self.stack)<self.max_size:
            self.stack.append(x)

    def pop(self) -> int:
        return self.stack.pop() if len(self.stack) else -1

    def increment(self, k: int, val: int) -> None:
        if len(self.stack)<k:
            k=len(self.stack)
        for i in range(k):
            self.stack[i]+=val
