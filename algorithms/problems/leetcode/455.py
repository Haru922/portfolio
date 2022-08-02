class Solution:
    def findContentChildren(self, g: List[int], s: List[int]) -> int:
        children = 0
        s.sort()
        for greed in sorted(g):
            for i, size in enumerate(s):
                if size >= greed:
                    children += 1
                    s.pop(i)
                    break
        return children
