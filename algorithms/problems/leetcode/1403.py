class Solution:
    def minSubsequence(self, nums: List[int]) -> List[int]:
        subsequence = []
        total, sub_total = sum(nums), 0
        for num in sorted(nums, reverse=True):
            subsequence.append(num)
            sub_total += num
            total -= num
            if sub_total > total:
                break
        return subsequence
