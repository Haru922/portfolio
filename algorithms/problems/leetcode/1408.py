class Solution:
    def stringMatching(self, words: List[str]) -> List[str]:
        answer = []
        words.sort(key=len)
        for i in range(len(words)-1):
            for j in range(i+1, len(words)):
                if words[i] in words[j]:
                    answer.append(words[i])
                    break
        return answer
