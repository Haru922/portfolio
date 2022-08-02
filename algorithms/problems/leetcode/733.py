class Solution:
    def floodFill(self, image: List[List[int]], sr: int, sc: int, newColor: int) -> List[List[int]]:
        stack = [(sr, sc)]
        color = image[sr][sc]
        if color != newColor:
            while stack:
                row, col = stack.pop()
                if row > 0 and image[row-1][col] == color:
                    stack.append((row-1, col))
                if row < len(image)-1 and image[row+1][col] == color:
                    stack.append((row+1, col))
                if col > 0 and image[row][col-1] == color:
                    stack.append((row, col-1))
                if col < len(image[0])-1 and image[row][col+1] == color:
                    stack.append((row, col+1))
                image[row][col] = newColor
        return image
