class Solution:
    def minOperations(self, boxes: str) -> List[int]:
        moves = []
        for i, _ in enumerate(boxes):
            move = 0
            for j, box in enumerate(boxes):
                if box == '1':
                    move += abs(i-j)
            moves.append(move)
        return moves
