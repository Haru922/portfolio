class Solution:
    def arrangeCoins(self, n: int) -> int:
        start, end = 1, n
        while start <= end:
            mid = (start+end)//2
            current = mid*(mid+1)//2
            if current == n:
                return mid
            elif current < n:
                start = mid+1
            else:
                end = mid-1
        return end
