class Solution:
    def findKthPositive(self, arr: List[int], k: int) -> int:
        if k <= arr[0]-1:
            return k

        k -= arr[0]-1

        for i in range(len(arr)-1):
            missing = arr[i+1]-arr[i]-1
            if k <= missing:
                return arr[i]+k
            k -= missing

        return arr[-1]+k
