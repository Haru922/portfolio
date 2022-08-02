class Solution:
    def minDepth(self, root: TreeNode) -> int:
        if not root:
            return 0
        min_depth = float('inf')
        nodes = [(root, 1)]
        while nodes:
            node, depth = nodes.pop()
            children = [node.left, node.right]
            if not any(children):
                min_depth = min(min_depth, depth)
            for child in children:
                if child:
                    nodes.append((child, depth+1))
        return min_depth
