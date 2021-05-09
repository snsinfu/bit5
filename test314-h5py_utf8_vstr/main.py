import h5py
import numpy as np


with h5py.File("_output.h5", "w") as store:
    # h5py does not predefine the dtype for variable-length strings. One needs to
    # define one as a "special dtype."
    # https://docs.h5py.org/en/2.9.0/strings.html#variable-length-utf-8
    # https://docs.h5py.org/en/stable/special.html#variable-length-strings
    vstr_dtype = h5py.special_dtype(vlen=str)
    names = ["apple", "banana", "cherry", "dorian"]

    # Creating dataset by passing the list as data=names causes an error:
    # TypeError: No conversion path for dtype: dtype('<U6')
    # So, we pass shape and dtype here and assign the data later.
    dataset = store.create_dataset("names", shape=(len(names), ), dtype=vstr_dtype)
    dataset[:] = names

    # And this works. Seemingly create_dataset ignores dtype argument and tries
    # to deduce dtype from data argument if it is specified.
    store.create_dataset("alt_names", data=np.array(names, dtype=vstr_dtype))

# Check the output of h5py -rlv _output.h5. It should output something like
# the following:
#
# alt_names                Dataset {4/4}
#     Location:  1:1400
#     Links:     1
#     Storage:   32 logical bytes, 64 allocated bytes, 50.00% utilization
#     Type:      variable-length null-terminated UTF-8 string
# names                    Dataset {4/4}
#     Location:  1:800
#     Links:     1
#     Storage:   32 logical bytes, 64 allocated bytes, 50.00% utilization
#     Type:      variable-length null-terminated UTF-8 string
#
# Note the Type: fields. The string data is stored correctly as null-terminated
# UTF-8 strings.
