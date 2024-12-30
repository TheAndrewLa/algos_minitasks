import random
from recordclass import recordclass

rand_set = set()

TreapNode = recordclass('TreapNode', ['position', 'priority', 'count', 'value', 'sub_value', 'left', 'right'])

def newNode(position: int, value: int):
    priority = random.randrange(1, 1000)

    while priority in rand_set:
        priority = random.randrange(1, 1000)

    rand_set.add(priority)

    return TreapNode(position, random.randrange(1000, 9999), 1, value, value, None, None)


class Treap:
    def __init__(self, value):
        self.__root = newNode(0, value)

    def insert(self, index: int , value: int):
        new = newNode(index, value)
        t1, t2 = Treap._split_by_size(self.__root, index - 1)

        self.__root = Treap._merge(Treap._merge(t1, new), t2)

    def remove(self, index: int):
        t1, r = Treap._split_by_size(self.__root, index - 1)
        _, t2 = Treap._split_by_size(r, 1)

        self.__root = Treap._merge(t1, t2)

    def sum(self, start: int, end: int) -> int:
        # split -> get_sum -> merge
        t1, t2 = Treap._split_by_size(self.__root, start - 1)
        t3, t4 = Treap._split_by_size(t2, end - start + 1)

        val = t3.sub_value

        self.__root = Treap._merge(t1, Treap._merge(t3, t4))

        return val

    @staticmethod
    def _split_by_size(node: TreapNode, key):
        if not node:
            return (None, None)

        c_left = 0 if (not node.left) else node.left.count

        if c_left >= key:
            t1, t2 = Treap._split_by_size(node.left, key)

            node.left = t2
            Treap._update(node)

            return (t1, node)
        else:
            t1, t2 = Treap._split_by_size(node.right, key - c_left - 1)

            node.right = t1
            Treap._update(node)

            return (node, t2)

    @staticmethod
    def _update(node: TreapNode):
        assert node is not None

        l_count = 0 if not node.left else node.left.count
        r_count = 0 if not node.right else node.right.count

        l_value = 0 if not node.left else node.left.value
        r_value = 0 if not node.right else node.right.value

        node.count = 1 + l_count + r_count
        node.sub_value = l_value + r_value + node.value

    @staticmethod
    def _merge(t1: TreapNode, t2: TreapNode):
        if not t1:
            return t2
        elif not t2:
            return t1

        if t1.priority < t2.priority:
            t1.right = Treap._merge(t1.right, t2)
            return t1
        else:
            t2.left = Treap._merge(t1, t2.left)
            return t2


a = Treap(10)
a.insert(1, 11)
a.insert(2, 12)
a.insert(3, 13)
a.insert(4, 14)
a.insert(6, 16)
a.insert(5, 15)

print(a.sum(2, 3))

a.remove(2)
a.remove(3)

print(a.sum(2, 3))

