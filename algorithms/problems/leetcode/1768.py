class Solution:
    def mergeAlternately(self, word1: str, word2: str) -> str:
        merged_word=''
        merged_length=max(len(word1),len(word2))
        for i in range(merged_length):
            if i<len(word1):
                merged_word+=word1[i]
            if i<len(word2):
                merged_word+=word2[i]
        return merged_word
