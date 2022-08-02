class Solution:
    def minimumAbsDifference(self, arr: List[int]) -> List[List[int]]:
        arr.sort()
        min_diff=arr[-1]-arr[0]
        for i in range(len(arr)-1):
            if min_diff>arr[i+1]-arr[i]:
                min_diff=arr[i+1]-arr[i]
                min_arr=[]
                min_arr.append([arr[i],arr[i+1]])
            elif min_diff==arr[i+1]-arr[i]:
                min_arr.append([arr[i],arr[i+1]])
        return min_arr
