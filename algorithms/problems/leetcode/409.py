class Solution:
    def longestPalindrome(self, s: str) -> int:
        longest = 0
        count = {}
        for c in s:
            if c not in count:
                count[c] = 0
            count[c] += 1
        for c in count:
            longest += count[c] // 2 * 2
            if longest % 2 == 0 and count[c] % 2:
                longest += 1
        return longest
