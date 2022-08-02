class Solution:
    def numUniqueEmails(self, emails: List[str]) -> int:
        counts = {} 
        for email in emails:
            address, domain = email.split(sep='@')
            if '+' in address:
                address, _ = address.split('+', 1)
            address = address.replace('.', '')
            address = address+'@'+domain
            counts[address] = counts.get(address, 0)+1
        return len(counts)
