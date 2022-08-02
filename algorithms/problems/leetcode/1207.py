class Solution:
    def uniqueOccurrences(self, arr: List[int]) -> bool:
        occurrences={}
        for num in arr:
            if num not in occurrences:
                occurrences[num]=0
            occurrences[num]+=1
        return len(occurrences.values())==len(set(occurrences.values()))
