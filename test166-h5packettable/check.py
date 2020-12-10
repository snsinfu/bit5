import h5py


with h5py.File("_output.h5", "r") as output:
    data = output["stream"][:]
    print("dtype", data.dtype)
    print("records:")
    for rec in data:
        print(rec)

# - h5py loads simple dataset of compound values as numpy structured array
# - Packet table is just an unbounded 1D dataset
