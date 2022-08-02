class Solution:
    def countPoints(self, points: List[List[int]], queries: List[List[int]]) -> List[int]:
        return [sum(((point_x-query_x)**2 + (point_y-query_y)**2) ** 0.5 <= radius for point_x, point_y in points) for query_x, query_y, radius in queries]
