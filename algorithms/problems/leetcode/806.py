class Solution:
    def numberOfLines(self, widths: List[int], s: str) -> List[int]:
        line,line_cnt=0,1
        for c in s:
            width=widths[ord(c)-ord('a')]
            if line+width<=100:
                line+=width
            else:
                line_cnt+=1
                line=width
        return [line_cnt,line]
