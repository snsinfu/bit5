"""
Wilson's theorem: Let n > 1 be a natural number. Then,

    (n - 1)! = -1  (mod n)  if and only if n is prime;
    (n - 1)! = 0   (mod n)  if and only if n is composite.

Therefore, the expression ((n - 1)! ** 2 % n) is the indicator of n being prime.
This is not useful in C++ because (n - 1)! ** 2 is too large to fit in built-in
integral types but super useful for golfing in Python.

The following code prinnts prime numbers <= 127 using Wilson's theorem.
"""
upper_bound = 127
fact = 1

for n in range(1, upper_bound + 1):
    if fact % n:
        print(n)
    fact *= n * n
