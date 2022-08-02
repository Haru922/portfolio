class Solution:
    def sumEvenAfterQueries(self, nums: List[int], queries: List[List[int]]) -> List[int]:
        answer = []
        even_sum = sum([num for num in nums if num%2 == 0])
                
        for num, idx in queries:
            if nums[idx]%2 == 0:
                even_sum -= nums[idx]
            nums[idx] += num
            if nums[idx]%2 == 0:
                even_sum += nums[idx]
            answer.append(even_sum)
            
        return answer
