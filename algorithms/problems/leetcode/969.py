class Solution:
    def pancakeSort(self, arr: List[int]) -> List[int]:
        def flip(target):
            sequence.append(target+1)
            for i in range((target+1)//2):
                arr[i], arr[target-i] = arr[target-i], arr[i]
        
        sequence = []
        
        for i in range(len(arr)):
            target = arr.index(max(arr[:len(arr)-i]))
            if target == len(arr)-1-i:
                continue
            flip(target)
            flip(len(arr)-1-i)
            
        return sequence
