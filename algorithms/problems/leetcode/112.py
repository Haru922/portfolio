class Solution:
    def hasPathSum(self, root: TreeNode, targetSum: int) -> bool:
        if root:
            stack = [[root, root.val]]
            while stack:
                node, node_sum = stack.pop()
                if not node.left and not node.right:
                    if node_sum == targetSum:
                        return True
                if node.left:
                    stack.append([node.left, node_sum+node.left.val])
                if node.right:
                    stack.append([node.right, node_sum+node.right.val])
        return False
