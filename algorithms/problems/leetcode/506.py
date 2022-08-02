class Solution:
    def findRelativeRanks(self, score: List[int]) -> List[str]:
        ranks = sorted(score, reverse=True)
        for i, rank in enumerate(ranks):
            if i >= 3:
                score[score.index(rank)] = str(i+1)
            elif i == 0:
                score[score.index(ranks[0])] = 'Gold Medal'
            elif i == 1:
                score[score.index(ranks[1])] = 'Silver Medal'
            elif i == 2:
                score[score.index(ranks[2])] = 'Bronze Medal'
        return score
