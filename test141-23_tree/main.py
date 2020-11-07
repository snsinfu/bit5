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
    root = insert(root, 90)

    root = insert(root, 35)
    root = insert(root, 15)
    root = insert(root, 75)
    root = insert(root, 65)
    root = insert(root, 45)
    root = insert(root, 85)
    root = insert(root, 95)

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

    # Split the node into two sibling nodes having lower and upper key.
    # Median key is promoted to the parent node. This can make the parent
    # node over-keyed (i.e., having three keys). We need to fix that.
    lower, median, upper = node.values
    parent = node.parent

    lower_sibling = Node(lower, parent)
    upper_sibling = Node(upper, parent)

    if parent is None:
        parent = Node(median)
        parent.children = [lower_sibling, upper_sibling]
        lower_sibling.parent = parent
        upper_sibling.parent = parent
        return parent

    if len(parent.children) == 2:
        # OK:
        #   p           p
        #  [k]        [k,m]
        #  / \   =>   / | \
        # *   n      *  s  n
        v, = parent.values
        a, b = parent.children
        if a is node:
            parent.values = [median, v]
            parent.children = [lower_sibling, upper_sibling, b]
        else:
            parent.values = [v, median]
            parent.children = [a, lower_sibling, upper_sibling]
    else:
        # Fix:
        #                                   |
        #                                   p
        #    |             |               [k]
        #    p             p              /   \
        #  [h,k]        [h,k,m]         [h]   [m]
        #  / | \   =>   / | | \    =>   / \   / \
        # *  *  n      *  * s  n       *   * s   n
        u, v = parent.values
        a, b, c = parent.children
        if a is node:
            proxy_1 = Node(median, parent)
            proxy_2 = Node(v, parent)
            parent.values = [u]
            parent.children = [proxy_1, proxy_2]
            proxy_1.children = [lower_sibling, upper_sibling]
            proxy_2.children = [b, c]
        elif b is node:
            proxy_1 = Node(u, parent)
            proxy_2 = Node(v, parent)
            parent.values = [median]
            parent.children = [proxy_1, proxy_2]
            proxy_1.children = [a, lower_sibling]
            proxy_2.children = [upper_sibling, c]
        else:
            proxy_1 = Node(u, parent)
            proxy_2 = Node(median, parent)
            parent.values = [v]
            parent.children = [proxy_1, proxy_2]
            proxy_1.children = [a, b]
            proxy_2.children = [lower_sibling, upper_sibling]

    return parent


def split(node):
    """
    Split over-keyed node.
    """
    assert len(node.values) == 3

    lower, median, upper = node.values
    left, middle, right = node.children

    sibling = Node(lower, parent)
    node.values = [upper]

    sibling.children = [left]
    node.children = [right]

    if node.parent is None:
        parent = Node(median)
        parent.children = [sibling, node]
        sibling.parent = node.parent = parent
        return parent

    if len(node.parent.children) == 1:
        node.parent.children = [sibling, node]
        node.parent.values.append(median)
        node.parent.values.sort()
    else:
        a, b = node.parent.children
        if a is node:
            node.parent.children = [sibling, node, b]
        else:
            node.parent.children = [a, sibling, node]
        node.parent.values.append(median)
        node.parent.values.sort()


    if len(node.parent.values) == 3:
        split(node.parent)

    return node.parent


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
