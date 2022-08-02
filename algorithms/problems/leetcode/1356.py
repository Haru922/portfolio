class Solution:
    def sortByBits(self, arr: List[int]) -> List[int]:
        return sorted(arr,key=lambda _: (bin(_).count('1'),_))
