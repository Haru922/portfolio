class Solution:
    def findComplement(self, num: int) -> int:
        bit = 1
        while bit <= num:
            num ^= bit
            bit <<= 1
        return num
