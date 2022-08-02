class Solution:
    def longestWord(self, words: List[str]) -> str:
        def validate_word(word: str) -> bool:
            if word=='':
                return True
            return False if len(word) not in word_dict or word not in word_dict[len(word)] else validate_word(word[:-1])
        word_dict={}
        return_word=[]
        for word in words:
            if len(word) not in word_dict:
                word_dict[len(word)]=[]
            word_dict[len(word)].append(word)
        longest=max(word_dict.keys())
        while longest:
            for word in word_dict[longest]:
                if validate_word(word[:-1]):
                    return_word.append(word)
            if len(return_word):
                break
            while longest:
                longest-=1
                if longest in word_dict:
                    break
        return min(return_word) if len(return_word) else ''
