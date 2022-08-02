class Solution:
    def decrypt(self, code: List[int], k: int) -> List[int]:
        decrypted=[0]*len(code)
        if k:
            d=1 if k>0 else -1
            for i in range(len(code)):
                for j in range(d,k+d,d):
                    decrypted[i]+=code[(i+j)%len(code)]
        return decrypted
