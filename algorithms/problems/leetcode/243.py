class Solution:
    def shortestDistance(self, wordsDict: List[str], word1: str, word2: str) -> int:
        word1_pos=word2_pos=-1
        shortest=len(wordsDict)
        for i in range(len(wordsDict)):
            if wordsDict[i]==word1:
                word1_pos=i
            elif wordsDict[i]==word2:
                word2_pos=i
            if word1_pos>-1 and word2_pos>-1:
                shortest=min(shortest,abs(word1_pos-word2_pos))
        return shortest
