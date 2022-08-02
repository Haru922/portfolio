class Solution:
    def isSubsequence(self, s: str, t: str) -> bool:
        cur = 0
        for c in t:
            if cur >= len(s):
                break
            elif s[cur] == c:
                cur += 1
        return True if cur == len(s) else False
