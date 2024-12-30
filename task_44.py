class Solution:

    def countSmaller(self, nums: list[int]) -> list[int]:
        tree = [0 for _ in range(0, 4 * (20000 + 1))]

        def update(node, left, right, index):
            if left == right:
                tree[node] += 1
                return

            mid = (left + right) // 2

            if index <= mid:
                update(2 * node, left, mid, index)
            else:
                update(2 * node + 1, mid + 1, right, index)
            
            tree[node] = tree[2 * node] + tree[2 * node + 1]

        def query(node, left, right, index_left, index_right):
            # Out of bounds
            if index_left > right or index_right < left:
                return 0

            # In bounds
            if index_left <= left and index_right >= right:
                return tree[node]

            mid = (left + right) // 2

            a = query(2 * node, left, mid, index_left, index_right)
            b = query(2 * node + 1, mid + 1, right, index_left, index_right)

            return a + b
        
        # Normalizing values (to work only with positive numbers)

        for i in range(len(nums)):
            nums[i] += (10 ** 4)

        answer = []
        nums_rev = nums[::-1]

        for i in nums_rev:
            answer.append(query(1, 0, 20000, 0, i - 1))
            update(1, 0, 20000, i)

        return answer[::-1]


a = Solution.countSmaller(Solution(), [5, 2, 6, 1])

print(a)
