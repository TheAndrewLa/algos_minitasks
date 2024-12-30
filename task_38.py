class Solution:
    def calculateMinimumHP(self, dungeon: list[list[int]]) -> int:
        MAX = __import__('math').inf

        height = len(dungeon)
        width = len(dungeon[0])

        health_table = []
        for _ in range(height):
            health_table.append([MAX for _ in range(width)])

        health_table[-1][-1] = max(1, 1 - dungeon[-1][-1])
        
        for i in range(height - 2, -1, -1):
            health_table[i][-1] = max(1, health_table[i + 1][-1] - dungeon[i][-1])
        
        for i in range(width - 2, -1, -1):
            health_table[-1][i] = max(1, health_table[-1][i + 1] - dungeon[-1][i])

        for i in range(height - 2, -1, -1):
            for j in range(width - 2, -1, -1):
                move_down = max(1, health_table[i + 1][j] - dungeon[i][j])
                move_right = max(1, health_table[i][j + 1] - dungeon[i][j])

                health_table[i][j] = min(move_down, move_right)

        return health_table[0][0]
    
print(Solution.calculateMinimumHP(Solution(), [[-2,-3,3],[-5,-10,1],[10,30,-5]]))


