class Solution:
    def kWeakestRows(self, mat: List[List[int]], k: int) -> List[int]:
        rows=[[idx,sum(row)] for idx,row in enumerate(mat)]
        rows.sort(key=lambda _: _[1])
        return [_[0] for _ in rows[:k]]
