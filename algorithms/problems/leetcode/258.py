class Solution:
    def addDigits(self, num: int) -> int:
        if num < 10:
            return num
        result = 0
        while num:
            result += num%10
            num //= 10
        return self.addDigits(result)
