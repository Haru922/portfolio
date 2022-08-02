class Solution:
    def maxIceCream(self, costs: List[int], coins: int) -> int:
        ice_cream = 0
        for cost in sorted(costs):
            if coins >= cost:
                coins -= cost
                ice_cream += 1
        return ice_cream
