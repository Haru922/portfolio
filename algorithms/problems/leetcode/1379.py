class Solution:
    def getTargetCopy(self, original: TreeNode, cloned: TreeNode, target: TreeNode) -> TreeNode:
        nodes = [(original, cloned)]
        clone = cloned
        while nodes:
            origin, clone = nodes.pop()
            if origin is target:
                break
            if origin.left:
                nodes.append((origin.left, clone.left))
            if origin.right:
                nodes.append((origin.right, clone.right))
        return clone
