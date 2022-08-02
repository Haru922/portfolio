class Solution:
    def sumOfLeftLeaves(self, root: TreeNode) -> int:
        nodes = [(root, False)]
        total = 0
        while nodes:
            node, can_add = nodes.pop()
            if can_add:
                if not node.left and not node.right:
                    total += node.val
            if node.left:
                nodes.append((node.left, True))
            if node.right:
                nodes.append((node.right, False))
        return total
