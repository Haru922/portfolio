class Solution:
    def postorderTraversal(self, root: TreeNode) -> List[int]:
        postorder = []
        if root:
            stack = [(root, root.val)]
            while stack:
                node, value = stack.pop()
                if node is None or (node.left is None and node.right is None):
                    postorder.append(value)
                else:
                    stack.append((None, value))
                    if node.right:
                        stack.append((node.right, node.right.val))
                    if node.left:
                        stack.append((node.left, node.left.val))
        return postorder
