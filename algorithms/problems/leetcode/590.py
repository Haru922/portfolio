class Solution:
    def postorder(self, root: 'Node') -> List[int]:
        stack=[]
        order=[]
        if root:
            stack.append(root)
        while stack:
            node=stack.pop()
            order.append(node.val)
            for child in node.children:
                stack.append(child)
        return reversed(order)
