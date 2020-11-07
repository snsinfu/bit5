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
        #  [v]        [v,m]
        #  / \   =>   / | \
        # *   n      *  s1 s2
        v, = parent.values
        a, b = parent.children
        if a is node:
            assert median <= v
            parent.values = [median, v]
            parent.children = [lower_sibling, upper_sibling, b]
        else:
            assert v <= median
            parent.values = [v, median]
            parent.children = [a, lower_sibling, upper_sibling]
    else:
        # Fix:
        #                                   |
        #                                   p
        #    |             |               [v]
        #    p             p              /   \
        #  [u,v]        [u,v,m]         [u]   [m]
        #  / | \   =>   / | | \    =>   / \   / \
        # *  *  n      *  * s1 s2      *   * s1  s2
        u, v = parent.values
        a, b, c = parent.children
        if a is node:
            assert median <= u <= v
            proxy_1 = Node(median, parent)
            proxy_2 = Node(v, parent)
            parent.values = [u]
            parent.children = [proxy_1, proxy_2]
            proxy_1.children = [lower_sibling, upper_sibling]
            proxy_2.children = [b, c]
            lower_sibling.parent = proxy_1
            upper_sibling.parent = proxy_1
            b.parent = proxy_2
            c.parent = proxy_2
        elif b is node:
            assert u <= median <= v
            proxy_1 = Node(u, parent)
            proxy_2 = Node(v, parent)
            parent.values = [median]
            parent.children = [proxy_1, proxy_2]
            proxy_1.children = [a, lower_sibling]
            proxy_2.children = [upper_sibling, c]
            a.parent = proxy_1
            lower_sibling.parent = proxy_1
            upper_sibling.parent = proxy_2
            c.parent = proxy_2
        else:
            assert u <= v <= median
            proxy_1 = Node(u, parent)
            proxy_2 = Node(median, parent)
            parent.values = [v]
            parent.children = [proxy_1, proxy_2]
            proxy_1.children = [a, b]
            proxy_2.children = [lower_sibling, upper_sibling]
            a.parent = proxy_1
            b.parent = proxy_1
            lower_sibling.parent = proxy_2
            upper_sibling.parent = proxy_2

    return parent


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
