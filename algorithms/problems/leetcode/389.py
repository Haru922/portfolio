class Solution:
    def findTheDifference(self, s: str, t: str) -> str:
        count_s, count_t = {}, {}
        for c in s:
            count_s[c] = count_s.get(c, 0)+1
        for c in t:
            count_t[c] = count_t.get(c, 0)+1
        for k in t:
            if (k not in count_s) or (count_t[k] > count_s[k]):
                return k
        return None
