class OrderedStream:

    def __init__(self, n: int):
        self.stream = ['']*n
        self.current = 0

    def insert(self, idKey: int, value: str) -> List[str]:
        self.stream[idKey-1] = value
        return_list = []
        while self.current<len(self.stream) and self.stream[self.current] != '':
            return_list.append(self.stream[self.current])
            self.current += 1
        return return_list
