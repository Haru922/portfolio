class Solution:
    def minCostToMoveChips(self, position: List[int]) -> int:
        even_count, odd_count = 0, 0
        for pos in position:
            if pos % 2:
                odd_count += 1
            else:
                even_count += 1
        return min(even_count, odd_count)
