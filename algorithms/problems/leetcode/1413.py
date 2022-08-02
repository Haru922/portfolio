class Solution:
    def minStartValue(self, nums: List[int]) -> int:
        sum_value=0
        min_value=nums[0]
        for num in nums:
            sum_value+=num
            min_value=min(min_value,sum_value)
        return 1 if min_value>0 else min_value*-1+1
