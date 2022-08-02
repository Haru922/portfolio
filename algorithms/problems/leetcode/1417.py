class Solution:
    def reformat(self, s: str) -> str:
        nums=[]
        chars=[]
        return_string=''
        for c in s:
            nums.append(c) if c.isdigit() else chars.append(c)
        if abs(len(nums)-len(chars))<2:
            if len(nums)>len(chars):
                first,second=nums,chars
            else:
                first,second=chars,nums
            return_string=first.pop() if len(first)!=len(second) else ''
            for i in range(len(second)):
                return_string+=second.pop()+first.pop()
        return return_string
