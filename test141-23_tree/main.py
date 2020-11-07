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
    # root = insert(root, 40)  # FIXME!

    walk(root)


def walk(root, indent=0):
    print("  " * indent + str(root.values))
    for child in root.children:
        walk(child, indent + 1)


def insert(node, value):
    """
    Insert `value` to the 2-3 tree whose root is `node`. Returns the root node
    of the 2-3 tree after insertion.
    """
    if len(node.values) == 1:
        return insert_1(node, value)

    if len(node.values) == 2:
        return insert_2(node, value)

    assert False


def insert_1(node, value):
    threshold, = node.values

    if node.children:
        left, right = node.children
        if value < threshold:
            if left:
                insert(left, value)
            else:
                node.children[0] = Node(value, node)
        else:
            if right:
                insert(right, value)
            else:
                node.children[1] = Node(value, node)
        return node

    node.values.append(value)
    node.values.sort()
    return node


def insert_2(node, value):
    lower, upper = node.values

    if node.children:
        left, middle, right = node.children
        if value < lower:
            if left:
                insert(left, value)
            else:
                node.children[0] = Node(value, node)
        elif value < upper:
            if middle:
                insert(middle, value)
            else:
                node.children[1] = Node(value, node)
        else:
            if right:
                insert(right, value)
            else:
                node.children[2] = Node(value, node)
        return node

    node.values.append(value)
    node.values.sort()
    lower, median, upper = node.values

    if node.parent is None:
        parent = Node(median)
        attach_child(parent, Node(lower))
        attach_child(parent, Node(upper))
        return parent

    parent = node.parent
    detach_child(parent, node)
    parent = insert(parent, median)
    attach_child(parent, Node(lower))
    attach_child(parent, Node(upper))
    return parent


def attach_child(node, child):
    node.children.append(child)
    child.parent = node


def detach_child(node, child):
    # This breaks everything! I need to rethink how to properly promote median,
    # detach a node and split it into new nodes.

    for i, ch in enumerate(node.children):
        if ch is child:
            node.children[i] = None
            child.parent = None
            return

    assert False


class Node:
    def __init__(self, value, parent=None):
        self.parent = parent
        self.children = []
        self.values = [value]


main()
