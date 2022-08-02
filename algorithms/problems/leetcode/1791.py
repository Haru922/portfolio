class Solution:
    def findCenter(self, edges: List[List[int]]) -> int:
        visited={}
        for edge in edges:
            for v in edge:
                if v not in visited:
                    visited[v]=True
                else:
                    center=v
                    break
        return center
