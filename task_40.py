class Solution:
    def mergeStones(self, stones: list[int], k: int) -> int:
        inf = __import__('math').inf

        # Checking that we can merge all piles into one performing merge by k consecutive piles
        
        if (len(stones) - 1) % (k - 1) != 0:
            return -1

        # Calculating an array of prefix sums

        pref_sum = [0]
        for i in stones:
            pref_sum.append(pref_sum[-1] + i)

        # Function which allow us to take cost of merging from i-th to j-th pile
        
        def calculate_merge_cost(i: int, j: int):
            assert j >= i
            assert (j - i) % (k - 1) == 0
            return pref_sum[j] - pref_sum[i]
        
        # Creating 2D table

        n = len(stones)


print(Solution.mergeStones(None, [3, 2, 4, 1], 2))
print(Solution.mergeStones(None, [3, 5, 1, 2, 6], 3))
