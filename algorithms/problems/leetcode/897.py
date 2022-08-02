class Solution:
    def inorder(self, root: TreeNode):
        if root:
            yield from self.inorder(root.left)
            yield root.val
            yield from self.inorder(root.right)
            
    def increasingBST(self, root: TreeNode) -> TreeNode:
        new_root = node = TreeNode(None)
        for val in self.inorder(root):
            node.right = TreeNode(val)
            node = node.right
        return new_root.right
