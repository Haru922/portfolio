class Solution:
    def fizzBuzz(self, n: int) -> List[str]:
        fizz_buzz=['']*n
        for i in range(n):
            s=''
            n=i+1
            if n%3==0:
                s+='Fizz'
            if n%5==0:
                s+='Buzz'
            fizz_buzz[i]=s if s else str(n)
        return fizz_buzz
