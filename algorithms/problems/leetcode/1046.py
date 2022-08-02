class Solution:
    def lastStoneWeight(self, stones: List[int]) -> int:
        for i in range(len(stones)):
            stones[i]*=-1
        heapq.heapify(stones)
        while len(stones)>1:
            first=heapq.heappop(stones)
            second=heapq.heappop(stones)
            if first!=second:
                heapq.heappush(stones,first-second)
        return -heapq.heappop(stones) if stones else 0
