class Solution:
    def areAlmostEqual(self, s1: str, s2: str) -> bool:
        if set(s1) != set(s2):
            return False
        diff = 0
        for i in range(len(s1)):
            if s1[i] != s2[i]:
                diff += 1
        return True if diff in [0, 2] else False
