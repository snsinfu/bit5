PREFIX = "("
SUFFIX = ")"


def bwt(text):
    text = PREFIX + text + SUFFIX
    shifts = [text[i:] + text[:i] for i in range(len(text))]
    shifts.sort()
    return "".join(shift[-1] for shift in shifts)


def inverse_bwt(text):
    table = [""] * len(text)
    for i in range(len(text)):
        table = [ch + row for row, ch in zip(table, text)]
        table.sort()
    for row in table:
        if row.endswith(SUFFIX):
            return row[1:-1]
    return None


source = "was it a car or a cat i saw"
code = bwt(source)
recovered = inverse_bwt(code)

print(f"   '{source}'")
print(f"-> '{code}'")
print(f"-> '{recovered}'")
