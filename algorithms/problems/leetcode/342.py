class Solution:
    def isPowerOfFour(self, n: int) -> bool:
        return self.isPowerOfFour(n//4) if n%4==0 and n else n==1
