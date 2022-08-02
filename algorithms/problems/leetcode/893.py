class Solution:
    def numSpecialEquivGroups(self, A: List[str]) -> int:
        groups = set() 
        for word in A:
            count = [0] * 52
            for i, c in enumerate(word):
                count[ord(c)-ord('a')+26*(i%2)] += 1
            groups.add(tuple(count))
        return len(groups)
