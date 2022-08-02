class Solution:
    def binaryGap(self, n: int) -> int:
        binary_gap, previous = 0, 0
        for i, b in enumerate(bin(n)[2:]):
            if b == '1':
                binary_gap = max(binary_gap, i-previous)
                previous = i
        return binary_gap
