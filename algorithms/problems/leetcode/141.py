class Solution:
    def hasCycle(self, head: ListNode) -> bool:
        if not head:
            return False
        node1, node2 = head, head.next
        while node1 != node2:
            if not node2 or not node2.next:
                return False
            node1 = node1.next
            node2 = node2.next.next
        return True
