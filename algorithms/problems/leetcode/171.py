class Solution:
    def titleToNumber(self, columnTitle: str) -> int:
        sum=0
        for title in columnTitle:
            sum=sum*26+ord(title)-ord('A')+1
        return sum
