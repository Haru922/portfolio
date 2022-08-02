class Solution:
    def numWaterBottles(self, numBottles: int, numExchange: int) -> int:
        count=empty=numBottles
        while empty>=numExchange:
            new=empty//numExchange
            empty%=numExchange
            count+=new
            empty+=new
        return count
