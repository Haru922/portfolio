class Solution:
    def reverseOnlyLetters(self, S: str) -> str:
        S=list(S)
        start,end=0,len(S)-1
        while start<end:
            for i in range(start,len(S)):
                if S[i].isalpha():
                    break
            start=i
            for i in range(end,-1,-1):
                if S[i].isalpha():
                    break
            end=i
            if start<end:
                S[start],S[end]=S[end],S[start]
                start+=1
                end-=1
        return ''.join(S)
