class Solution:
    def isSameTree(self, p: TreeNode, q: TreeNode) -> bool:
        stack=[(p,q)]
        while stack:
            nd1,nd2=stack.pop(0)
            if nd1 and nd2 and nd1.val==nd2.val:
                stack.extend([(nd1.left,nd2.left),(nd1.right,nd2.right)])
            elif nd1 or nd2:
                break
        return False if nd1 or nd2 else True
