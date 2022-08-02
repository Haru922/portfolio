class Solution:
    def reformatNumber(self, number: str) -> str:
        number=number.replace(' ','').replace('-','')
        new_number=[]
        tail=[]
        length=len(number)
        if length%3==1:
            length=length-4
            tail.append(number[-4:-2])
            tail.append(number[-2:])
        elif length%3==2:
            length=length-2
            tail.append(number[-2:])
        for i in range(length//3):
            new_number.append(number[i*3:i*3+3])
        return '-'.join(new_number+tail)
