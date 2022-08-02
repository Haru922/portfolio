class Solution:
    def canPermutePalindrome(self, s: str) -> bool:
        odd_count=0
        for c in set(s):
            char_count=s.count(c)
            if char_count%2:
                odd_count+=1
            if odd_count>1:
                break
        return True if odd_count<2 else False
