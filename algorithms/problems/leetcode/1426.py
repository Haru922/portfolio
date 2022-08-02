class Solution:
    def countElements(self, arr: List[int]) -> int:
        count=0
        nums=set(arr)
        for num in arr:
            if num+1 in nums:
                count+=1
        return count
