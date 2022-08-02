class Solution:
    def islandPerimeter(self, grid: List[List[int]]) -> int:
        border=0
        rows=len(grid)
        cols=len(grid[0])
        for row in range(rows):
            for col in range(cols):
                if grid[row][col]:
                    border=border+1 if row==0 else border if grid[row-1][col] else border+1
                    border=border+1 if row==rows-1 else border if grid[row+1][col] else border+1
                    border=border+1 if col==0 else border if grid[row][col-1] else border+1
                    border=border+1 if col==cols-1 else border if grid[row][col+1] else border+1
        return border
