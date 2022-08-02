class Solution:
    def cloneTree(self, root: 'Node') -> 'Node':
        if not root:
            return None
        clone_tree = Node(root.val, None)
        nodes = [(root, clone_tree)]
        while nodes:
            root, clone = nodes.pop(0)
            for i, child in enumerate(root.children):
                clone.children.append(Node(child.val, None))
                nodes.append((child, clone.children[i]))
        return clone_tree
