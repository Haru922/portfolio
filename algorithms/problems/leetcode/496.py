class Solution:
    def nextGreaterElement(self, nums1: List[int], nums2: List[int]) -> List[int]:
        i=0
        greater_map={}
        stack=[]
        while i<len(nums2):
            if len(stack) and nums2[i]>stack[-1]:
                greater_map[stack[-1]]=nums2[i]
                stack.pop()
            else:
                stack.append(nums2[i])
                i+=1
        while len(stack):
            greater_map[stack.pop()]=-1
        return [greater_map[_] for _ in nums1]
