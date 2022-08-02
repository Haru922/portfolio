class Solution:
    def subdomainVisits(self, cpdomains: List[str]) -> List[str]:
        domain_counts={}
        for cpdomain in cpdomains:
            cpdomain=cpdomain.split()
            domains=cpdomain[1].split('.')
            for i in range(len(domains)):
                domain='.'.join(domains[i:])
                if domain not in domain_counts:
                    domain_counts[domain]=0
                domain_counts[domain]+=int(cpdomain[0])
        return [str(domain_counts[_])+' '+_ for _ in domain_counts]
