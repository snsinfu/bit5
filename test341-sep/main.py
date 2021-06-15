import sys


def main():
    for line in sys.stdin:
        digits = line.strip()
        sep_digits = insert_separators(digits)
        print(sep_digits)


def insert_separators(digits, sep=",", step=3):
    # Is there a more elegant way?
    sep_digits = ""
    input_end = len(digits)
    beg = 0
    end = input_end % step
    while end <= input_end:
        if len(sep_digits):
            sep_digits += sep
        sep_digits += digits[beg:end]
        beg = end
        end = end + step
    return sep_digits


main()
