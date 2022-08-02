class FindElements:

    def __init__(self, root: TreeNode):
        self.root = root
        self.values = set()
        if self.root.val == -1:
            self.root.val = 0
            queue = [self.root]
            while queue:
                node = queue.pop(0)
                self.values.add(node.val)
                if node.left:
                    node.left.val = node.val * 2 + 1
                    queue.append(node.left)
                if node.right:
                    node.right.val = node.val * 2 + 2
                    queue.append(node.right)
            
    def find(self, target: int) -> bool:
        return target in self.values
