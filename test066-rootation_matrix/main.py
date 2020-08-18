import numpy as np


X = np.array([
    [0.097627, 0.430379, 0.205527],
    [0.0897664, -0.15269, 0.291788],
    [-0.124826, 0.783546, 0.927326],
    [-0.233117, 0.58345, 0.0577898],
    [0.136089, 0.851193, -0.857928],
])

Y = np.array([
    [ 0.00125262, 0.4903831, -0.06393117],
    [-0.2712589, -0.03944207, -0.21439677],
    [-0.06095781, 0.95336987, -0.76827547],
    [ 0.36560811, 0.47623628, -0.1113517],
    [ 0.62498998, 0.57372133, 0.85405673],
])


# Compute the rotation matrix that superimposes X onto Y with minimum error.
u, s, vh = np.linalg.svd(X.T @ Y)
rot = u @ vh

assert abs(np.linalg.det(rot) - 1) < 1e-6

rmsd = np.sqrt(np.mean(np.linalg.norm(Y @ rot.T - X, axis=1)**2))
assert rmsd < 0.1


# Derive rotation axis and angle from the matrix.
# https://en.wikipedia.org/wiki/Rotation_matrix
u, s, vh = np.linalg.svd(rot - np.eye(3))
tol = 1e-6
axis = vh[np.abs(s) < tol][0]

cos = (np.trace(rot) - 1) / 2
angle = np.arccos(cos)


# Reconstruct rotation matrix from the axis and the angle. (Is there a way
# to algorithmically determine the sign of the sine?)
sin = np.sqrt(1 - cos**2)
cross = np.array([
    [0, axis[2], -axis[1]],
    [-axis[2], 0, axis[0]],
    [axis[1], -axis[0], 0],
])
rot_recons = cos * np.eye(3) + sin * cross + (1 - cos) * axis[:, None] * axis[None, :]

assert np.max(np.abs(rot_recons - rot)) < 1e-6


print(f"angle = {angle:g}")
