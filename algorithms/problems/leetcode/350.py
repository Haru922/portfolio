class Solution:
    def intersect(self, nums1: List[int], nums2: List[int]) -> List[int]:
        count = {}
        intersect = []
        nums = nums1 if len(nums1) <= len(nums2) else nums2
        for num in nums:
            if num not in count:
                count[num] = 0
            count[num] += 1
        nums = nums1 if len(nums1) > len(nums2) else nums2
        for num in nums:
            if num in count and count[num] > 0:
                intersect.append(num)
                count[num] -= 1
        return intersect
