class Solution:
    def buddyString(self, a: str, b: str) -> bool:
        result=False
        if len(a)==len(b):
            idx=[0]*2
            diff_count=0
            for i in range(len(a)):
                if a[i]!=b[i]:
                    diff_count+=1
                    if diff_count>2:
                        break
                    idx[diff_count-1]=i
            if diff_count==0:
                for c in a:
                    if a.count(c)>=2:
                        result=True
                        break
            elif diff_count==2 and a[idx[0]]==b[idx[1]] and a[idx[1]]==b[idx[0]]:
                result=True
        return result
