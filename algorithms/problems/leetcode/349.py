class Solution:
    def intersection(self, nums1: List[int], nums2: List[int]) -> List[int]:
        return [_ for _ in set(nums1) if _ in set(nums2)]
