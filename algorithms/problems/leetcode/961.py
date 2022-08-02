class Solution:
    def repeatedNTimes(self, A: List[int]) -> int:
        num_count={}
        for num in A:
            if num not in num_count:
                num_count[num]=0
            num_count[num]+=1
        for num in num_count:
            if num_count[num]==len(A)//2:
                break
        return num
