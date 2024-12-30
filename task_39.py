class Solution:
    def isMatch(self, s: str, p: str) -> bool:
        def charMatch(i: int, j: int):
            return s[i] == p[j] or p[j] == '.'

        table = []
        for i in range(len(s) + 1):
            table.append([False for _ in range(len(p) + 1)])

        if (charMatch(0, 0)):
            table[0][0] = True
        else:
            table[0][0] = False

        # propogating value when we find '*' symbol

        for i in range(len(p)):
            if p[i] == '*':
                table[0][i + 1] = table[0][i - 1]

        for i in range(len(s)):
            for j in range(len(p)):
                if p[j] == '*':
                    assert j > 0

                    if table[i + 1][j - 1] == True:
                        table[i + 1][j + 1] = True

                    elif charMatch(i, j - 1):
                        table[i + 1][j + 1] = table[i][j + 1]

                elif charMatch(i, j):
                    table[i + 1][j + 1] = table[i][j]

        return table[len(s)][len(p)]

print(Solution.isMatch(None, "aa", "a"))
print(Solution.isMatch(None, "aa", "a*"))
print(Solution.isMatch(None, "ab", ".*"))

print(Solution.isMatch(None, "abbbbba", "ab*a"))
print(Solution.isMatch(None, "abbbab", "ab*a"))
