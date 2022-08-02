class Solution;
    def isPowerOfTwo(self, n: int) -> bool:
        return n&(-n)==n if n else False
