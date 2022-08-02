class Solution:
    def reverseStr(self, s: str, k: int) -> str:
        return_string=''
        index=0
        while index<len(s):
            return_string+=''.join(reversed(s[index:index+k]))+''.join(s[index+k:index+k*2])
            index+=k*2
        return return_string
