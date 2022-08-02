class Solution:
    def commonChars(self, A: List[str]) -> List[str]:
        common_chars=list(A[0])
        for word in A[1:]:
            check_list=[]
            for c in word:
                if c in common_chars:
                    check_list.append(c)
                    common_chars.remove(c)
            common_chars=check_list
        return common_chars
