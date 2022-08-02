class Solution:
    def findJudge(self, N: int, trust: List[List[int]]) -> int:
        judge=-1
        trust_scores=[0]*(N+1)
        for a,b in trust:
            trust_scores[a]-=1
            trust_scores[b]+=1
        for person,trust_score in enumerate(trust_scores[1:],1):
            if trust_score==N-1:
                judge=person
                break
        return judge
