class Solution:
    def sumRootToLeaf(self, root: TreeNode) -> int:
        queue=[]
        answer=0
        queue.append(root)
        while len(queue):
            node=queue.pop(0)
            if node.left:
                node.left.val=node.val<<1|node.left.val
                queue.append(node.left)
            if node.right:
                node.right.val=node.val<<1|node.right.val
                queue.append(node.right)
            if node.left is None and node.right is None:
                answer+=node.val
        return answer
