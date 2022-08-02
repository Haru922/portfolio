class Solution:
    def numberOfMatches(self, n: int) -> int:
        cnt=0
        while n>1:
            odd=1 if n%2 else 0
            n//=2
            cnt+=n
            n+=odd
        return cnt
