class Solution:
    def containsNearbyDuplicate(self, nums: List[int], k: int) -> bool:
        count = {}
        for i, num in enumerate(nums):
            if num in count:
                if i-count[num] <= k:
                    return True
            count[num]=i
        return False
