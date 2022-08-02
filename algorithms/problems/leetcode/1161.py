class Solution:
    def maxLevelSum(self, root: TreeNode) -> int:
        nodes = [(root, 1)]
        level_sum = {}
        while nodes:
            node, level = nodes.pop()
            level_sum[level] = level_sum.get(level, 0) + node.val
            if node.left:
                nodes.append((node.left, level+1))
            if node.right:
                nodes.append((node.right, level+1))
        return max(level_sum, key=level_sum.get)
