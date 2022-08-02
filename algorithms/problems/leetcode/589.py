class Solution:
    def preorder(self, root: 'Node') -> List[int]:
        stack=[]
        preorder=[]
        if root:
            stack.append(root)
        while stack:
            node=stack.pop()
            preorder.append(node.val)
            for i in range(len(node.children)-1,-1,-1):
                stack.append(node.children[i])
        return preorder
