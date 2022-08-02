class Solution:
    def isAnagram(self, s: str, t: str) -> bool:
        counter={}
        is_anagram=True
        for c in s:
            if c not in counter:
                counter[c]=0
            counter[c]+=1
        for c in t:
            if c not in counter or counter[c]<1:
                is_anagram=False
                break
            counter[c]-=1
        if is_anagram:
            for c in counter:
                if counter[c]:
                    is_anagram=False
                    break
        return is_anagram
