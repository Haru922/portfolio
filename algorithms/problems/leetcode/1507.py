class Solution:
    def reformatDate(self, date: str) -> str:
        months=['Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec']
        day,month,year=date.split()
        day='0'+day[0] if len(day)<4 else day[:2]
        return f'{year}-{month.index(month)+1:02d}-{int(day):02d}'
