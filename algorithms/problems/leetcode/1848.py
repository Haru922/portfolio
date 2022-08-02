class Solution:
    def getMinDistance(self, nums: List[int], target: int, start: int) -> int:
        min_distance = len(nums)
        for i, num in enumerate(nums):
            if num == target:
                min_distance = min(min_distance, abs(i-start))
        return min_distance
