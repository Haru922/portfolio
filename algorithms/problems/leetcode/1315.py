class Solution:
    def sumEvenGrandparent(self, root: TreeNode) -> int:
        nodes, answer = [(root, 0, 0)], 0
        while nodes:
            node, parent, grand_parent = nodes.pop()
            if grand_parent and grand_parent%2 == 0:
                answer += node.val
            if node.left:
                nodes.append((node.left, node.val, parent))
            if node.right:
                nodes.append((node.right, node.val, parent))
        return answer
