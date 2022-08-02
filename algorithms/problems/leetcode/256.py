class Solution:
    def minCost(self, costs: List[List[int]]) -> int:
        paint_costs={}
        def get_cost(house,color):
            if (house,color) not in paint_costs:
                paint_cost=costs[house][color]
                if house<len(costs)-1:
                    if color==0:
                        paint_cost+=min(get_cost(house+1,1),get_cost(house+1,2))
                    elif color==1:
                        paint_cost+=min(get_cost(house+1,0),get_cost(house+1,2))
                    else:
                        paint_cost+=min(get_cost(house+1,0),get_cost(house+1,1))
                paint_costs[(house,color)]=paint_cost
            return paint_costs[(house,color)]
        return min(get_cost(0,0),get_cost(0,1),get_cost(0,2))
