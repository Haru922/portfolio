class Solution:
    def findShortestSubArray(self, nums: List[int]) -> int:
        count = {}
        max_count = 0
        shortest = 50001
        for i, num in enumerate(nums):
            if num not in count:
                count[num] = []
            count[num].append(i)
            max_count = max(max_count, len(count[num]))
        for num in count:
            if len(count[num]) == max_count:
                shortest = min(shortest, count[num][-1]-count[num][0]+1)
        return shortest
