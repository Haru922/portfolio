class Solution:
    def twoSumBSTs(self, root1: TreeNode, root2: TreeNode, target: int) -> bool:
        nodes, elements = [root1], set()
        while nodes:
            node = nodes.pop()
            elements.add(target-node.val)
            if node.left:
                nodes.append(node.left)
            if node.right:
                nodes.append(node.right)
        nodes = [root2]
        while nodes:
            node = nodes.pop()
            if node.val in elements:
                return True
            if node.left:
                nodes.append(node.left)
            if node.right:
                nodes.append(node.right)
        return False
