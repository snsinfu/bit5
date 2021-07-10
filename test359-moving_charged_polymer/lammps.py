import numpy as np


trajectory = np.loadtxt("_output.dat")
trajectory = trajectory.reshape(len(trajectory), -1, 4)
n_frames, n_atoms, _ = trajectory.shape

x_lo, y_lo, z_lo = trajectory[:, :, :3].min(axis=1).min(axis=0)
x_hi, y_hi, z_hi = trajectory[:, :, :3].min(axis=1).min(axis=0)

with open("_output.data", "w") as data:
    data.write("Output\n")
    data.write("\n")
    data.write(f"{n_atoms} atoms\n")
    data.write(f"{n_atoms - 1} bonds\n")
    data.write("\n")
    data.write("1 atom types\n")
    data.write("1 bond types\n")
    data.write("\n")
    data.write(f"{x_lo:g} {x_hi:g} xlo xhi\n")
    data.write(f"{y_lo:g} {y_hi:g} ylo yhi\n")
    data.write(f"{z_lo:g} {z_hi:g} zlo zhi\n")
    data.write("\n")
    data.write("Atoms\n")
    data.write("\n")
    for i, (x, y, z, q) in enumerate(trajectory[0]):
        atom_id = i + 1
        data.write(f"{atom_id} 1 {x:g} {y:g} {z:g}\n")
    data.write("\n")
    data.write("Bonds\n")
    data.write("\n")
    for i in range(1, n_atoms):
        bond_id = i
        atom_pair = i, i + 1
        data.write(f"{bond_id} 1 {atom_pair[0]} {atom_pair[1]}\n")

with open("_output.dump", "w") as dump:
    for frame, snapshot in enumerate(trajectory):
        dump.write("ITEM: TIMESTEP\n")
        dump.write(f"{frame}\n")
        dump.write("ITEM: BOX BOUNDS xx yy zz\n")
        dump.write(f"{x_lo} {x_hi}\n")
        dump.write(f"{y_lo} {y_hi}\n")
        dump.write(f"{z_lo} {z_hi}\n")
        dump.write("ITEM: NUMBER OF ATOMS\n")
        dump.write(f"{n_atoms}\n")
        dump.write("ITEM: ATOMS id x y z q\n")
        for i, (x, y, z, q) in enumerate(snapshot):
            atom_id = i + 1
            dump.write(f"{atom_id} {x:g} {y:g} {z:g} {q:g}\n")
