class Solution:
    def isPalindrome(self, head: ListNode) -> bool:
        values = []
        while head:
            values.append(head.val)
            head = head.next
        return values == values[::-1]
