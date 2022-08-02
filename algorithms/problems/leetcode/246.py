class Solution:
    def isStrobogrammatic(self, num: str) -> bool:
        strobo = ''
        for i in range(len(num)-1,-1,-1):
            if num[i] in '018':
                strobo += num[i]
            elif num[i] is '6':
                strobo += '9'
            elif num[i] is '9':
                strobo += '6'
            else:
                return False
        return strobo == num
