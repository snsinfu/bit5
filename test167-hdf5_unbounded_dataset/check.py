import h5py


with h5py.File("_output.h5", "r") as output:
    state_history = output["state_history"]
    print(state_history[::100])

