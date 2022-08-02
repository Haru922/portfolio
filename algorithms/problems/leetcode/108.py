class Solution:
    def sortedArrayToBST(self, nums: List[int]) -> TreeNode:
        root=None
        if nums:
            mid=len(nums)//2 if len(nums)%2 else len(nums)//2-1
            root=TreeNode(nums[mid])
            root.left=self.sortedArrayToBST(nums[:mid])
            root.right=self.sortedArrayToBST(nums[mid+1:])
        return root
