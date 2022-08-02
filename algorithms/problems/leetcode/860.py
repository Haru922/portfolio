class Solution:
    def lemonadeChange(self, bills: List[int]) -> bool:
        received={}
        received[5]=received[10]=received[20]=0
        can_change=True
        for bill in bills:
            received[bill]+=1
            change=bill-5
            while change>0 and can_change:
                if 20<=change and received[20]:
                    change-=20
                    received[20]-=1
                elif 10<=change and received[10]:
                    change-=10
                    received[10]-=1
                elif received[5]:
                    change-=5
                    received[5]-=1
                else:
                    can_change=False
            if not can_change:
                break
        return can_change
