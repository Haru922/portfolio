class Solution:
    def generatePossibleNextMoves(self, currentState: str) -> List[str]:
        next_moves=[]
        for i in range(len(currentState)-1):
            if currentState[i]=='+' and currentState[i]==currentState[i+1]:
                next_moves.append(currentState[:i]+'--'+currentState[i+2:])
        return next_moves
