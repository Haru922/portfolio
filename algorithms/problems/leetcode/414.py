class Solution:
    def thirdMax(self, nums: List[int]) -> int:
        arr = sorted(list(set(nums)))
        return arr[-3] if len(arr)>=3 else arr[-1]
