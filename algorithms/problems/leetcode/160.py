class Solution:
    def getIntersectionNode(self, headA: ListNode, headB: ListNode) -> ListNode:
        point_a=headA
        point_b=headB
        while point_a!=point_b:
            point_a=point_a.next if point_a else headB
            point_b=point_b.next if point_b else headA
        return point_a
