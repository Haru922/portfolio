class Solution:
    def smallestRangeI(self, A: List[int], K: int) -> int:
        min_value=min(A)
        max_value=max(A)
        min_value=max_value if min_value+K>max_value else min_value+K
        if max_value>min_value:
            max_value=min_value if max_value-K<min_value else max_value-K
        return max_value-min_value
