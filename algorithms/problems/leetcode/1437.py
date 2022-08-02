class Solution:
    def kLengthApart(self, nums: List[int], k: int) -> bool:
        prev = -1
        for i, num in enumerate(nums):
            if num == 1:
                if prev != -1:
                    if k > i-prev-1:
                        return False
                prev = i
        return True
