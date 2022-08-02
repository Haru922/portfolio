class Solution:
    def maxDepth(self, root: TreeNode) -> int:
        max_depth=0
        if root:
            max_depth=max(self.maxDepth(root.left),self.maxDepth(root.right))+1
        return max_depth
