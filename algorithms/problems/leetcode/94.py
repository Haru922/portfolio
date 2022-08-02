class Solution:
    def inorderTraversal(self, root: TreeNode) -> List[int]:
        inorder = []
        if root:
            stack = [(root, root.val)]
            while stack:
                node, value = stack.pop()
                if node is None or (node.left is None and node.right is None):
                    inorder.append(value)
                else:
                    if node.right:
                        stack.append((node.right, node.right.val))
                    stack.append((None, node.val))
                    if node.left:
                        stack.append((node.left, node.left.val))
        return inorder
