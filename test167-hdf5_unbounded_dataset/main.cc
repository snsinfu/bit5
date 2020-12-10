#include <cstddef>
#include <cstdint>
#include <vector>

#include <hdf5.h>


using std::int8_t;
using std::size_t;


int main()
{
    constexpr hsize_t state_size = 20;

    hid_t file = H5Fcreate("_output.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    hsize_t max_dims[2] = { H5S_UNLIMITED, state_size };
    hsize_t cur_dims[2] = { 0, state_size };
    hsize_t mem_dims[2] = { 1, state_size };
    hsize_t chunk_dims[2] = { 100, state_size };

    hid_t dataspace = H5Screate_simple(2, cur_dims, max_dims);
    hid_t memspace = H5Screate_simple(2, mem_dims, mem_dims);
    hid_t dataset_props = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_chunk(dataset_props, 2, chunk_dims);
    H5Pset_shuffle(dataset_props);
    H5Pset_deflate(dataset_props, 6);

    hid_t dataset = H5Dcreate2(
        file,
        "state_history",
        H5T_STD_I8LE,
        dataspace,
        H5P_DEFAULT,
        dataset_props,
        H5P_DEFAULT
    );

    int8_t state[state_size] = {};

    for (long i = 0; i < 10000; i++) {
        cur_dims[0]++;
        H5Dset_extent(dataset, cur_dims);

        H5Sset_extent_simple(dataspace, 2, cur_dims, max_dims);
        H5Sselect_none(dataspace);

        hsize_t offset[2] = { cur_dims[0] - 1, 0 };
        hsize_t count[2] = { 1, state_size };
        H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL, count, NULL);

        state[size_t(i) % state_size] ^= 1;
        state[size_t(i) % state_size] ^= state[size_t(i + 1) % state_size];

        H5Dwrite(dataset, H5T_NATIVE_SCHAR, memspace, dataspace, H5P_DEFAULT, state);
    }

    H5Dclose(dataset);
    H5Pclose(dataset_props);
    H5Sclose(memspace);
    H5Sclose(dataspace);
    H5Fclose(file);
}
