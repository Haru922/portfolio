class Solution:
    def licenseKeyFormatting(self, S: str, K: int) -> str:
        S=S.replace('-','')[::-1]
        key=S[0].upper() if len(S) else ''
        for i in range(1,len(S)):
            key=key+S[i].upper() if i%K else key+'-'+S[i].upper()
        return key[::-1]
