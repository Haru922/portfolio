class Solution:
    def luckyNumbers (self, matrix: List[List[int]]) -> List[int]:
        lucky_num, min_row, max_col = [], [], []
        for row in matrix:
            min_row.append(min(row))
        for i in range(len(matrix[0])):
            max_col.append(max([matrix[row][i] for row in range(len(matrix))]))
        for num in min_row:
            if num in max_col:
                lucky_num.append(num)
        return lucky_num
