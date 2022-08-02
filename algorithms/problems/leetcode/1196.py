class Solution:
    def maxNumberOfApples(self, arr: List[int]) -> int:
        weight, count = 0, 0
        arr.sort()
        for _, apple in enumerate(arr):
            weight += apple
            if weight <= 5000:
                count += 1
        return count
