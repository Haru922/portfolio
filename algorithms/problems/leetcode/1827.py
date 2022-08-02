class Solution:
    def minOperations(self, nums: List[int]) -> int:
        operations = 0
        for i in range(1, len(nums)):
            if nums[i] <= nums[i-1]:
                diff = (nums[i-1]+1)-nums[i]
                operations += diff if diff else 1
                nums[i] = nums[i-1]+1
        return operations
