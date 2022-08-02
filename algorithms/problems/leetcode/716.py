class MaxStack:

    def __init__(self):
        self.stack = []

    def push(self, x: int) -> None:
        max_value = max(x, self.stack[-1][1]) if self.stack else x
        self.stack.append((x, max_value))

    def pop(self) -> int:
        return self.stack.pop()[0]
        
    def top(self) -> int:
        return self.stack[-1][0]

    def peekMax(self) -> int:
        return self.stack[-1][1]

    def popMax(self) -> int:
        pops = []
        while self.stack:
            pop = self.stack.pop()
            if pop[0] == pop[1]:
                break
            pops.append(pop)
        while pops:
            self.push(pops.pop()[0])
        return pop[0]
