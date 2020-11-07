# I'd like to understand finger tree. Then, I guess I should be familiar with
# 2-3 tree, the underlying data structure of finger tree.
#
# https://en.wikipedia.org/wiki/2%E2%80%933_tree
# https://cathyatseneca.gitbooks.io/data-structures-and-algorithms/content/2-3_trees/
# https://www.geeksforgeeks.org/2-3-trees-search-and-insert/
# https://www.slideshare.net/sandpoonia/23-tree


def main():
    root = Node(50)
    root = insert(root, 30)
    root = insert(root, 10)
    root = insert(root, 70)
    root = insert(root, 60)
    root = insert(root, 40)
    root = insert(root, 80)
    #root = insert(root, 90) # FIXME

    walk(root)


def walk(root, indent=0):
    print("  " * indent + str(root.values))
    for child in root.children:
        walk(child, indent + 1)


def insert(root, value):
    """
    Insert value to a 2-3 tree. Returns the root node of the 2-3 tree after
    insertion, which can potentially be re-created.
    """
    node = do_insert(find_leaf(root, value), value)
    while node.parent:
        node = node.parent
    return node


def do_insert(node, value):
    node.values.append(value)
    node.values.sort()

    if len(node.values) == 2:
        return node

    lower, median, upper = node.values

    sibling = Node(lower, node.parent)
    node.values = [upper]

    if node.parent is None:
        parent = Node(median)
        parent.children = [sibling, node]
        sibling.parent = parent
        node.parent = parent
        return parent

    if len(node.parent.children) == 1:
        node.parent.children = [sibling, node]
    else:
        a, b = node.parent.children
        if a is node:
            node.parent.children = [sibling, node, b]
        else:
            node.parent.children = [a, sibling, node]

    return do_insert(node.parent, median)


def find_leaf(node, value):
    if not node.children:
        return node

    if len(node.children) == 2:
        left, right = node.children
        threshold, = node.values
        if value < threshold:
            child = left
        else:
            child = right

    if len(node.children) == 3:
        left, middle, right = node.children
        lower, upper = node.values
        if value < lower:
            child = left
        elif value < upper:
            child = middle
        else:
            child = right

    return find_leaf(child, value)


class Node:
    def __init__(self, value, parent=None):
        self.parent = parent
        self.children = []
        self.values = [value]


main()
