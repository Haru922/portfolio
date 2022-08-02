class Solution:
    def closestValue(self, root: TreeNode, target: float) -> int:
        node, closest_val = root, root.val
        while node:
            closest_val = min(node.val, closest_val, key=lambda _: abs(target-_))
            node = node.right if node.val < target else node.left
        return closest_val
