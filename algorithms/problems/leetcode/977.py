class Solution:
    def sortedSquares(self, nums: List[int]) -> List[int]:
        length=len(nums)
        sorted_squares=[0]*length
        left_pointer=0
        right_pointer=length-1
        for i in range(length-1,-1,-1):
            if abs(nums[left_pointer])>abs(nums[right_pointer]):
                num=nums[left_pointer]
                left_pointer+=1
            else:
                num=nums[right_pointer]
                right_pointer-=1
            sorted_squares[i]=num*num
        return sorted_squares
