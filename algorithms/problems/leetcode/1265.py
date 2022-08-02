class Solution:
    def printLinkedListInReverse(self, head: 'ImmutableListNode') -> None:
        if head.getNext():
            self.printLinkedListInReverse(head.getNext())
        head.printValue()
