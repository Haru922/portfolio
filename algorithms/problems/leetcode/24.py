class Solution:
    def swapPairs(self, head: ListNode) -> ListNode:
    
        def swap_pair(head: ListNode) -> ListNode:
            first = head
            if first and first.next:
                second = first.next
                first.next = swap_pair(second.next)
                second.next = first
                head = second
            return head
        
        return swap_pair(head)
