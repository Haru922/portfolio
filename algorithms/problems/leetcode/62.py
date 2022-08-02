class Solution:
    def uniquePaths(self, m: int, n: int) -> int:
        path = [[1] * n for _ in range(m)]
        for i in range(1, m):
            for j in range(1, n):
                path[i][j] = path[i][j-1] + path[i-1][j]
        return path[m-1][n-1]
