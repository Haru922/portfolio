class Solution:
    def backspaceCompare(self, s: str, t: str) -> bool:
        def new_string(s : str) -> str:
            new_s = []
            for c in s:
                if c != '#':
                    new_s.append(c)
                elif new_s:
                    new_s.pop()
            return ''.join(new_s)
        return new_string(s) == new_string(t)
