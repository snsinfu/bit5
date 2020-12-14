# https://www.redblobgames.com/grids/hexagons/

def main():
    #
    #                       axial         cube
    #  / \ / \ / \ /        __ +x       +y   +x
    # | 0 |   |   |         \             \ /
    #  \ / \ / \ / \ /       +y            |
    #   |   |   |   |                      +z
    #  / \ / \ / \ / \
    # |   | B |   |   |     B: axial (0, 2) cube (0, -2, 2)
    #  \ / \ / \ / \ /
    #   |   |   | A |       A: axial (1, 3) cube (1, -4, 3)
    #  / \ / \ / \ / \
    #
    A = 1, 3
    B = 0, 2

    pA = axial_to_cube(A)
    pB = axial_to_cube(B)

    print(pA, pB)
    print(sub(pA, pB))

    # C should be (-1, 1) in axial coordinates.
    pC = sub(pB, sub(pA, pB))
    C = cube_to_axial(pC)
    print(C)


def axial_to_cube(pax):
    x, z = pax
    y = -x - z
    return x, y, z


def cube_to_axial(pc):
    x, y, z = pc
    return x, z


def sub(p1, p2):
    x1, y1, z1 = p1
    x2, y2, z2 = p2
    return x1 - x2, y1 - y2, z1 - z2


main()
