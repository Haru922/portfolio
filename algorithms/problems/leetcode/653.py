class Solution:
    def findTarget(self, root: TreeNode, k: int) -> bool:
        def inorder(root):
            if root:
                inorder(root.left)
                self.vals.append(root.val)
                inorder(root.right)

        self.vals = []
        inorder(root)
        low, high = 0, len(self.vals)-1
        while low < high:
            total = self.vals[low]+self.vals[high]
            if total == k:
                return True
            elif total > k:
                high -= 1
            else:
                low += 1
        return False
