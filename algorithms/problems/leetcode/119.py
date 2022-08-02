class Solution:
    pascal = [[1]]
    def getRow(self, rowIndex: int) -> List[int]:
        if len(self.pascal) > rowIndex:
            return self.pascal[rowIndex]
        prev_row = self.getRow(rowIndex-1)
        row = [1] * (len(prev_row)+1)
        for i in range(1, len(row)-1):
            row[i] = prev_row[i-1]+prev_row[i]
        self.pascal.append(row)
        return row
