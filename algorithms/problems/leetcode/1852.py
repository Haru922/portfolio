class Solution:
    def distinctNumbers(self, nums: List[int], k: int) -> List[int]:
        counts = {}
        distinct_numbers = []
        for num in nums[0:k]:
            counts[num] = counts.get(num, 0) + 1
        distinct_numbers.append(len(counts))
        for i in range(1, len(nums)-k+1):
            counts[nums[i-1]] -= 1
            if counts[nums[i-1]] == 0:
                del counts[nums[i-1]]
            counts[nums[i+k-1]] = counts.get(nums[i+k-1], 0) + 1
            distinct_numbers.append(len(counts))
        return distinct_numbers
