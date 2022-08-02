class Solution:
    def isSymmetric(self, root: TreeNode) -> bool:
        symmetric=True
        stack=[]
        stack.append(root.left)
        stack.append(root.right)
        while stack and symmetric:
            right=stack.pop()
            left=stack.pop()
            if left and right:
                if left.val!=right.val:
                    symmetric=False
                else:
                    stack.append(left.left)
                    stack.append(right.right)
                    stack.append(left.right)
                    stack.append(right.left)
            elif left or right:
                symmetric=False
        return symmetric
