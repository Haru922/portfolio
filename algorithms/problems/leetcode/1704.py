class Solution:
    def halvesAreAlike(self, s: str) -> bool:
        mid=len(s)//2
        cnt=0
        for c in s[:mid]:
            cnt=cnt+1 if c in 'EeAaIiOoUu' else cnt
        for c in s[mid:]:
            cnt=cnt-1 if c in 'EeAaIiOoUu' else cnt
        return False if cnt else True
