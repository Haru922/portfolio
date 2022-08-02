class Solution:
    def trimMean(self, arr: List[int]) -> float:
        arr.sort()
        n = len(arr)//20
        new_arr = arr[n:len(arr)-n]
        return sum(new_arr)/len(new_arr)
