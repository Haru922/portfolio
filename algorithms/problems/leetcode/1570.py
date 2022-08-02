class SparseVector:
    def __init__(self, nums: List[int]):
        self.nums = nums
        

    # Return the dotProduct of two sparse vectors
    def dotProduct(self, vec: 'SparseVector') -> int:
        product = 0
        for i, n in enumerate(self.nums):
            if n:
                product += n * vec.nums[i]
        return product
