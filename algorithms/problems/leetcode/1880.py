class Solution:
    def isSumEqual(self, firstWord: str, secondWord: str, targetWord: str) -> bool:
        li, fw = len(firstWord)-1, enumerate(firstWord)
        first_num = sum([(ord(c)-ord('a'))*(10**(li-i)) for i, c in fw])
        li, sw = len(secondWord)-1, enumerate(secondWord)
        second_num = sum([(ord(c)-ord('a'))*(10**(li-i)) for i, c in sw])
        li, tw = len(targetWord)-1, enumerate(targetWord)
        target_num = sum([(ord(c)-ord('a'))*(10**(li-i)) for i, c in tw])
        return target_num == (first_num+second_num)
