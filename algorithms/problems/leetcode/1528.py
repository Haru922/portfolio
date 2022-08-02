class Solution:
    def restoreString(self, s: str, indices: List[int]) -> str:
        new_string=['']*len(s)
        for index in range(len(s)):
            new_string[indices[index]]=s[index]
        return "".join(new_string)
