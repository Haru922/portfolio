class Solution:
    def mergeInBetween(self, list1: ListNode, a: int, b: int, list2: ListNode) -> ListNode:
        node=tail=list1
        for i in range(b,-1,-1):
            tail=tail.next
            if a-1:
                node=node.next
                a-=1
        node.next=list2
        node=list2
        while node.next:
            node=node.next
        node.next=tail
        return list1
