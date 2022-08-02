class Solution:
    def replaceDigits(self, s: str) -> str:
        s = list(s) 
        return ''.join([chr(ord(s[i-1])+int(s[i])) if i%2 else s[i] for i,c in enumerate(s)])
