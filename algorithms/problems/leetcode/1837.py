class Solution:
    def sumBase(self, n: int, k: int) -> int:
        sum_base = 0
        while n:
            n, mod = n // k, n % k
            sum_base += mod
        return sum_base
