class Solution:
    def countGoodRectangles(self, rectangles: List[List[int]]) -> int:
        rect = [min(length, width) for length,width in rectangles]
        return rect.count(max(rect))
