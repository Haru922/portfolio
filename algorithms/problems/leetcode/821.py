class Solution:
    def shortestToChar(self, s: str, c: str) -> List[int]:
        char_pos=[]
        shortest_distance=[]
        for i in range(len(s)):
            if s[i]==c:
                char_pos.append(i)
        for i in range(len(s)):
            min_distance=len(s)
            for pos in char_pos:
                if min_distance<abs(pos-i):
                    break
                min_distance=min(min_distance,abs(pos-i))
            shortest_distance.append(min_distance)
        return shortest_distance
