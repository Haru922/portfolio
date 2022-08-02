class Solution:
    def maximumUnits(self, boxTypes: List[List[int]], truckSize: int) -> int:
        max_units=0
        boxTypes.sort(key=lambda _: _[1],reverse=True)
        for box in boxTypes:
            if truckSize==0:
                break
            max_units+=min(box[0],truckSize)*box[1]
            truckSize-=min(box[0],truckSize)
        return max_units
