class Solution:
    def sortSentence(self, s: str) -> str:
        s = s.split()
        sorted_s = ['']*len(s)
        for word in s:
            sorted_s[int(word[-1])-1] = word[:-1]
        return ' '.join(sorted_s)
