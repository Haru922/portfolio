class Solution:
    def buildArray(self, target: List[int], n: int) -> List[str]:
        target_array = []
        cur = 0
        for i in range(1, n+1):
            if cur >= len(target):
                break
            target_array.append("Push")
            if target[cur] == i: 
                cur += 1
            else:
                target_array.append("Pop")
        return target_array
