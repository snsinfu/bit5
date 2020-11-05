# Linearly addressing lower triangular elements of a square matrix.
#
# Motivation: To pick a random lower-triangular element of a square matrix.
# It is wasteful to generate (i, j) pairs # until i > j is satisfied. If we
# generate a single linear address of a lower-triangular element and can
# convert it to the corresponding (i, j) pair, then random nunmber generation
# is only once.

n = 4
num_lower_elements = n * (n - 1) // 2

# Address mapping:
# *
# 0 *
# 1 2 *
# 3 4 5 *
# 6 7 8 9 *

# (i, j) -> address
print("Index to address:")

for i in range(n):
    for j in range(i):
        address = i * (i - 1) // 2 + j
        print(f"{i},{j}\t{address}")

# address -> (i, j)
print("Address to index:")

# See the address equation:
#
#   A = i (i - 1) / 2 + j
#
# Given A, we would like to solve the equation for i. Note here that i must
# be constant throughout j in [0, i). So if a function f(A) gives i, f must be
# constant over some range of A starting from A(i,0) to A(i,i-1).
#
# Maybe it is easy to focus on the lower boundary (j = 0)?
#
#   A = i (i - 1) / 2
#   i^2 - i - 2A = 0
#   i = floor( (1 + sqrt(1 + 8A)) / 2 ).
#
# So, this would be the function f(A) and this f should give the same i for
# different j's.

for address in range(num_lower_elements):
    a = (1 + 8 * address) ** 0.5
    i = int((a + 1) / 2)
    j = address - i * (i - 1) // 2
    assert i > j >= 0

    print(f"{address}\t{i},{j}")
