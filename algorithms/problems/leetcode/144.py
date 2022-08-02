class Solution:
    def preorderTraversal(self, root: TreeNode) -> List[int]:
        preorder = []
        if root:
            stack = [root]
            while stack:
                node = stack.pop()
                preorder.append(node.val)
                if node.right:
                    stack.append(node.right)
                if node.left:
                    stack.append(node.left)
        return preorder
