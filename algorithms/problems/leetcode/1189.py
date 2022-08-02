class Solution:
    def maxNumberOfBallons(self, text: str) -> int:
        ballons={}
        for c in 'ablno':
            ballons[c]=0
        for c in text:
            if c in ballons:
                ballons[c]+=1
        ballons['l']//=2
        ballons['o']//=2
        return min(ballons.values())
