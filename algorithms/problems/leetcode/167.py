class Solution:
    def twoSum(self, numbers: List[int], target: int) -> List[int]:
        low, high = 0, len(numbers)-1
        while low < high:
            sum_numbers = numbers[low]+numbers[high]
            if sum_numbers > target:
                high -= 1
            elif sum_numbers < target:
                low += 1
            else:
                break
        return [low+1, high+1]
