class Solution:
    def check(self, nums: List[int]) -> bool:
        start = 0
        for i in range(len(nums)-1):
            if nums[i] > nums[i+1]:
                start = i+1
                break
        prev = nums[start]
        for i in range(1, len(nums)):
            idx = (start+i)%len(nums)
            if prev > nums[idx]:
                return False
            prev = nums[idx]
        return True
