class Solution:
    def isSubtree(self, s: TreeNode, t: TreeNode) -> bool:
        def get_preorder(root: TreeNode) -> str:
            vals = []
            traversal = [root]
            while traversal:
                node = traversal.pop()
                vals.append(str(node.val))
                if node.right:
                    traversal.append(node.right)
                else:
                    vals.append('RN')
                if node.left:
                    traversal.append(node.left)
                else:
                    vals.append('LN')
            return ' ' + ' '.join(vals)
        return get_preorder(t) in get_preorder(s)
