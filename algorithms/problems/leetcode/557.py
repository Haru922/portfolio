class Solution:
    def reverseWords(self, s: str) -> str:
        new_string=s.split()
        for i in range(len(new_string)):
            new_string[i]=new_string[i][::-1]
        return ' '.join(new_string)
