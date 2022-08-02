class Solution:
    def findDisappearedNumbers(self, nums: List[int]) -> List[int]:
        count = {}
        answer = []
        for num in nums:
            count[num] = 1
        for i in range(1, len(nums)+1):
            if i not in count:
                answer.append(i)
        return answer
