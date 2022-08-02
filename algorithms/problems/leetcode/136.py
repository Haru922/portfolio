from collections import defaultdict
class Solution:
    def singleNumber(self, nums: List[int]) -> int:
        num_counts=defaultdict(int)
        for num in nums:
            num_counts[num]+=1
        for num in num_counts:
            if num_counts[num]==1:
                single_num=num
                break
        return single_num
