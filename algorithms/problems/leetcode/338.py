class Solution:
    def countBits(self, n: int) -> List[int]:
        mask = 0b11111111111111111
        counts = []
        for i in range(n+1):
            counts.append(bin(i&mask).count('1'))
        return counts
