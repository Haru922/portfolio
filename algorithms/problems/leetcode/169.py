class Solution:
    def majorityElement(self, nums: List[int]) -> int:
        num_counts={}
        majority_element=0
        for num in nums:
            if num not in num_counts:
                num_counts[num]=0
            num_counts[num]+=1
            if num_counts[num]>len(nums)//2:
                majority_element=num
                break
        return majority_element
