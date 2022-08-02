class Solution:
    def bstToGst(self, root: TreeNode) -> TreeNode:
        
        def inorder(root: TreeNode):
            if root:
                inorder(root.left)
                nodes.append(root)
                inorder(root.right)
                
        nodes = []
        inorder(root)
        
        prev = 0
        while nodes:
            node = nodes.pop()
            node.val += prev
            prev = node.val
            
        return root
