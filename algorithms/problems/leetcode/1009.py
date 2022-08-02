class Solution:
    def bitwiseComplement(self, N: int) -> int:
        if N == 0:
            return 1
        mask = N
        for i in range(6):
            mask |= mask>>(2**i)
        return N^mask
