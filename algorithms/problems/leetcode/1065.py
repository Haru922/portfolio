class Solution:
    def indexPairs(self, text: str, words: List[str]) -> List[List[int]]:
        pairs = []
        for word in words:
            for i, c, in enumerate(text):
                if text[i:i+len(word)] == word:
                    pairs.append([i, i+len(word)-1])
                    i += (len(word)-1)
        return sorted(pairs)
