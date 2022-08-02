class Solution:
    def maximumPopulation(self, logs: List[List[int]]) -> int:
        population = [0]*(2050-1950+1)
        for birth, death in logs:
            for i in range(birth-1950, death-1950):
                population[i] += 1
        return population.index(max(population))+1950
