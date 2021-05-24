source = "mississippi banana lorem ipsum quick brown fox jumps over the lazy dog"

counts = {}
for ch in source:
    counts[ch] = counts.get(ch, 0) + 1


codebook = "".join(
    ch for ch, freq in sorted(counts.items(), key=lambda pair: -pair[1])
)

ranks = {}
for rank, ch in enumerate(codebook):
    ranks[ch] = rank

alphabet = "abcdefghijklmnopqrstuvwxyz*"
output = "".join(alphabet[ranks[ch]] for ch in source)

print(f"source: '{source}'")
print(f"output: '{output}'")
print(f"codebook: '{codebook}'")
