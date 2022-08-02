class Solution:
    def arraySign(self, nums: List[int]) -> int:
        minus=0
        for num in nums:
            if num==0:
                return 0
            elif num<0:
                minus+=1
        return -1 if minus%2 else 1
