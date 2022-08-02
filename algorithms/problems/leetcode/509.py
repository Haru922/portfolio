class Solution:
    fib_nums={}
    fib_nums[0]=0
    fib_nums[1]=1
    def fib(self, n: int) -> int:
        if n in self.fib_nums:
            pass
        else:
            self.fib_nums[n]=self.fib(n-1)+self.fib(n-2)
        return self.fib_nums[n]
