class Solution:
    def pivotIndex(self, nums: List[int]) -> int:
        total, sub_total = sum(nums), 0
        for i, num in enumerate(nums):
            total -= num
            if sub_total == total:
                return i
            sub_total += num
        return -1
