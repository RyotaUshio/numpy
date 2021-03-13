import numpy as np

a = np.array([[[1, 0], [0, 1]], [[2, -1], [5, 4]]])
# a = np.array(range(24)).reshape(2, 3, 4)

for i in range(-1, a.shape[0]):
    i_ = i
    if i == -1:
        i = slice(None)
    for j in range(-1, a.shape[1]):
        j_ = j
        if j == -1:
            j = slice(None)
        for k in range(-1, a.shape[2]):
            k_ = k
            if k == -1:
                k = slice(None)

            print(f"a[{i_}, {j_}, {k_}] = {a[i, j, k].ravel()}")
