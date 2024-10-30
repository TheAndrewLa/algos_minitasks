from collections import namedtuple
import random

TreapNode = namedtuple('TreapNode', ['position', 'priority', 'count', 'value', 'left', 'right'])

def newNode(position: int, value: int) -> TreapNode:
    new = TreapNode()

    new.position = position
    new.priority = random.randrange(1, 1000) # REPLACE IT WITH CORRECT RANDOM
    new.count = 0
    new.value = value
    new.left = new.right = None

    return new


class Treap:
    def __init__(self, value):
        self.__root = newNode(0, value)

    def insert(self, index: int , value: int) -> None:
        new = newNode(index, value)
        t1, t2 = Treap._split_by_size(self.__root, index - 1)

        self.__root = Treap._merge(Treap._merge(t1, new), t2)


    def remove(self, index: int) -> None:
        t1, r = Treap._split_by_size(self.__root, index - 1)
        _, t2 = Treap._split_by_size(r, 1)

        self.__root = Treap._merge(t1, t2)

    def sum(self, start: int, end: int) -> int:
        pass

    # This is internal stuff below

    @staticmethod
    def _split_by_size(node: TreapNode, key):
        if not node:
            return (None, None)

        if not node.left:
            # What should I return here?????
            return (node, None)
        
        c_left = node.left.count
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
        node.value += l_value + r_value

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

