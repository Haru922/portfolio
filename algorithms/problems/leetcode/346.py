class MovingAverage:

    def __init__(self, size: int):
        self.elements = []
        self.size = size
        self.sum = 0

    def next(self, val: int) -> float:
        if len(self.elements) == self.size:
            self.sum -= self.elements.pop(0)
        self.elements.append(val)
        self.sum += val
        return self.sum / len(self.elements)
