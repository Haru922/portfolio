class Solution:
    def findLucky(self, arr: List[int]) -> int:
        prev=lucky=-1
        arr.sort()
        for i in range(len(arr)-1,-1,-1):
            if prev!=arr[i]:
                prev=arr[i]
                if arr.count(arr[i])==arr[i]:
                    lucky=arr[i]
                    break
        return lucky
