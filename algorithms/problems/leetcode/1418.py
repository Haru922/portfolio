class Solution:
    def displayTable(self, orders: List[List[str]]) -> List[List[str]]:
        display=[]
        order_log={}
        for order in orders:
            if order[2] not in order_log:
                order_log[order[2]]=[]
            order_log[order[2]].append(order[1])
        menus=[_ for _ in order_log.keys()]
        menus.sort()
        menus.insert(0,"Table")
        display.append(menus)
        tables=list(set([_[1] for _ in orders]))
        tables.sort(key=lambda x: int(x))
        for table_num in tables:
            table=[]
            table.append(table_num)
            for menu in menus[1:]:
                table.append(str(order_log[menu].count(table_num)))
            display.append(table)
        return display
