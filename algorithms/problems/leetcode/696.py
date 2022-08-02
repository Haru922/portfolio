class Solution:
    def countBinarySubstrings(self, s: str) -> int:
        bin_cnt, prev_cnt, cur_cnt = 0, 0, 1
        for i in range(1, len(s)):
            if s[i] != s[i-1]:
                bin_cnt += min(prev_cnt, cur_cnt)
                prev_cnt, cur_cnt = cur_cnt, 1
            else:
                cur_cnt += 1
        return bin_cnt + min(prev_cnt, cur_cnt)
