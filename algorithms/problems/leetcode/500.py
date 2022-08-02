class Solution:
    def findWords(self, words: List[str]) -> List[str]:
        find_words = []
        rows = ['qwertyuiopQWERTYUIOP',
                'asdfghjklASDFGHJKL',
                'zxcvbnmZXCVBNM']
        for word in words:
            find = True
            if word[0] in rows[0]:
                row = rows[0]
            elif word[0] in rows[1]:
                row = rows[1]
            else:
                row = rows[2]
            for c in set(word):
                if c not in row:
                    find = False
                    break
            if find:
                find_words.append(word)
        return find_words
