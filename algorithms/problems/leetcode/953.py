class Solution:
    def isAlienSorted(self, words: List[str], order: str) -> bool:
        alien_dict = {}
        for i, c in enumerate(order):
            alien_dict[c] = i
        for i in range(len(words)-1):
            for j in range(len(words[i])):
                if (j >= len(words[i+1])) or (alien_dict[words[i][j]] > alien_dict[words[i+1][j]]):
                    return False
                elif alien_dict[words[i][j]] < alien_dict[words[i+1][j]]:
                    break
        return True
