class Solution:
    def fairCandySwap(self, A: List[int], B: List[int]) -> List[int]:
        ans=[0]*2
        sum_a=sum(A)
        sum_b=sum(B)
        fair_candies=(sum_a+sum_b)//2
        for candy in A:
            target_candy=fair_candies-sum_a+candy
            if target_candy in B:
                ans[0],ans[1]=candy,target_candy
                break
        return ans
