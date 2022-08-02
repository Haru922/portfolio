class Solution:
    def countBalls(self, lowLimit: int, highLimit: int) -> int:
        boxes={}
        for ball in range(lowLimit,highLimit+1):
            box_num=sum([int(_) for _ in str(ball)])
            if box_num not in boxes:
                boxes[box_num]=0
            boxes[box_num]+=1
        return max(boxes.values())
