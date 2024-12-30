class NumArray:

    def __init__(self, nums: list[int]):
        self.__actual_len = len(nums)
        self.__array = [0 for _ in range(4 * len(nums))]

        def build_tree(array, vertex, left, right):
            if (left == right):
                self.__array[vertex] = array[left]
                return

            mid = (left + right) >> 1
            build_tree(array, vertex * 2, left, mid)
            build_tree(array, vertex * 2 + 1, mid + 1, right)

            self.__array[vertex] = self.__array[2 * vertex] + self.__array[2 * vertex + 1]
        
        build_tree(nums, 1, 0, self.__actual_len - 1)


    def update(self, index: int, val: int) -> None:
        
        def update_(array, vertex, left, right):
            print(array)

            if left == right:
                array[vertex] = val
                return

            mid = (left + right) >> 1

            if index <= mid:
                update_(array, 2 * vertex, left, mid)
            else:
                update_(array, 2 * vertex + 1, mid + 1, right)

            array[vertex] = array[2 * vertex] + array[2 * vertex + 1]

        update_(self.__array, 1, 0, self.__actual_len - 1)

        print(self.__array)

    def sumRange(self, left: int, right: int) -> int:

        def sum_(array, vertex, l, r, left, right):
            if l > r:
                return 0
            
            if l == left and r == right:
                return array[vertex]

            mid = (left + right) >> 1

            res = 0
            res += sum_(array, vertex * 2, l, min(mid, r), left, mid)
            res += sum_(array, vertex * 2 + 1, max(mid + 1, l), r, mid + 1, right)

            return res

        return sum_(self.__array, 1, left, right, 0, self.__actual_len - 1)

arr = NumArray([-1])
print(arr.sumRange(0, 0))
arr.update(0, 1)
print(arr.sumRange(0, 0))

arr2 = NumArray([1, 3, 5])
print(arr2.sumRange(0, 2))
arr2.update(1, 2)
print(arr2.sumRange(0, 2))
