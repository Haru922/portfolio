class TwoSum:

    def __init__(self):
        self.nums = {} 
        

    def add(self, number: int) -> None:
        self.nums[number] = self.nums.get(number, 0)+1
            

    def find(self, value: int) -> bool:
        for k in self.nums.keys():
            diff = value-k
            if diff != k:
                if diff in self.nums and k in self.nums:
                    return True
            elif diff in self.nums and self.nums[diff]>1:
                return True
        return False 
