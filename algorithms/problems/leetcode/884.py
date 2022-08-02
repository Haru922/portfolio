class Solution:
    def uncommonFromSentences(self, A: str, B: str) -> List[str]:
        count={}
        for word in A.split():
            if word not in count:
                count[word]=0
            count[word]+=1
        for word in B.split():
            if word not in count:
                count[word]=0
            count[word]+=1
        return [_ for _ in count if count[_]==1]
