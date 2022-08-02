class Solution:
    def truncateSentence(self, s: str, k: int) -> str:
        new_string=s.split()
        return ' '.join(new_string[:k])
