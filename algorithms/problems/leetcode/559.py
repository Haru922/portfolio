class Solution:
    def maxDepth(self, root: 'Node') -> int:
        max_depth = 0
        if root:
            nodes = [(root, 1)]
            while nodes:
                node, depth = nodes.pop()
                for child in node.children:
                    nodes.append((child, depth+1))
                max_depth = max(max_depth, depth)
        return max_depth
