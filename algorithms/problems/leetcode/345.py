class Solution:
    def reverseVowels(self, s: str) -> str:
        vowels = 'aeiouAEIOU'
        vstack = []
        new_s = ''
        for c in s:
            if c in vowels:
                vstack.append(c)
        for i, c in enumerate(s):
            new_s += vstack.pop() if c in vowels else c
        return new_s
