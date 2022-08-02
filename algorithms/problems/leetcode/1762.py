class Solution:
    def findBuildings(self, heights: List[int]) -> List[int]:
        buildings, prev_max = [], 0
        for i in range(len(heights)-1, -1, -1):
            if heights[i] > prev_max:
                buildings.append(i)
                prev_max = heights[i]
        return reversed(buildings)
