class Solution:
    def removeDuplicates(self, S: str) -> str:
        stack=[S[0]]
        for c in S[1:]:
            if stack and stack[-1]==c:
                stack.pop()
            else:
                stack.append(c)
        return ''.join(stack)
