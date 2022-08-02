class Solution:
    deleted = False
    def validPalindrome(self, s: str) -> bool:
        if len(s) < 2:
            return True 
        if s[0] != s[-1]:
            if self.deleted:
                return False
            else:
                self.deleted = True
                return self.validPalindrome(s[1:]) or self.validPalindrome(s[:-1])
        else:
            return self.validPalindrome(s[1:-1])
