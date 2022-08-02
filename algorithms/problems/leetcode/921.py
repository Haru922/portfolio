class Solution:
    def minAddToMakeValid(self, S: str) -> int:
        opening, add_cnt = 0, 0
        for c in S:
            opening += 1 if c == '(' else -1
            if opening < 0:
                add_cnt += 1
                opening += 1
        return add_cnt + opening
