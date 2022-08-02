class Solution:
    def relativeSortArray(self, arr1: List[int], arr2: List[int]) -> List[int]:
        counts, array, remaining = {}, [], []
        
        for num in arr1:
            if not num in arr2:
                remaining.append(num)
            else:
                counts[num] = counts.get(num, 0)+1
        remaining.sort()
                
        for num in arr2:
            array.extend([num]*counts[num])
        array.extend(remaining)
            
        return array
