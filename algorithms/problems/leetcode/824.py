class Solution:
    def toGoatLatin(self, S: str) -> str:
        latin_words=S.split()
        for i in range(len(latin_words)):
            if latin_words[i][0] not in 'aeiouAEIOU':
                latin_words[i]=latin_words[i][1:]+latin_words[i][0]
            latin_words[i]=latin_words[i]+'m'+'a'*(i+2)
        return ' '.join(latin_words)
