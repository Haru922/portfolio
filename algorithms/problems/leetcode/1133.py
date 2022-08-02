class Solution:
    def largestUniqueNumber(self, A: List[int]) -> int:
        unique=[_ for _ in set(A) if A.count(_)==1]
        return max(unique) if unique else -1
