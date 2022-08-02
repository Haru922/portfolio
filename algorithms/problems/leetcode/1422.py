class Solution:
    def maxScore(self, s: str) -> int:
        max_score=0
        for i in range(1,len(s)):
            score=s[:i].count('0')+s[i:].count('1')
            max_score=max(max_score,score)
        return max_score
