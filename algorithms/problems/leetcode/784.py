class Solution:
    def letterCasePermutation(self, S: str) -> List[str]:
        queue = [S[0].lower(), S[0].upper()] if S[0].isalpha() else [S[0]]
        for i in range(1, len(S)):
            permutation = []
            while queue:
                permutation.append(queue.pop(0))
            if S[i].isalpha():
                for s in permutation:
                    queue.append(s+S[i].lower())
                    queue.append(s+S[i].upper())
            else:
                for s in permutation:
                    queue.append(s+S[i])
        return queue
