class Solution:
    def wordPattern(self, pattern: str, s: str) -> bool:
        match, defined = {}, set()
        words = s.split()
        if len(words) == len(pattern):
            for i, c in enumerate(pattern):
                if c in match:
                    if match[c] != words[i]:
                        return False
                elif words[i] in defined:
                    return False
                match[c] = words[i]
                defined.add(words[i])
            return True
        return False
