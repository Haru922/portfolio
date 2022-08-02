class Solution:
    def isIsomorphic(self, s: str, t: str) -> bool:
        replace_dict = {}
        taken = set()
        for i, c in enumerate(s):
            if c in replace_dict:
                if replace_dict[c] != t[i]:
                    return False
            elif t[i] in taken:
                return False
            else:
                replace_dict[c] = t[i]
                taken.add(t[i])
        return True
