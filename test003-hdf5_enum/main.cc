#include <array>
#include <cstdint>
#include <random>
#include <string>

#include <hdf5.h>


int main()
{
    auto store = H5Fcreate("_output.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    auto datatype = H5Tenum_create(H5T_NATIVE_INT8);
    std::string enum_names[] {"zeroth", "first", "second", "third"};
    std::int8_t value = 0;
    for (auto const& name : enum_names) {
        H5Tenum_insert(datatype, name.c_str(), &value);
        value++;
    }

    hsize_t shape[] {30, 2};
    auto dataspace = H5Screate_simple(std::size(shape), shape, nullptr);
    auto filespace = H5Scopy(dataspace);

    hsize_t single[] {1, 1};
    auto memspace = H5Screate_simple(std::size(single), single, nullptr);

    auto dataset = H5Dcreate2(
        store, "/data", datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT
    );

    std::mt19937_64 random;
    std::uniform_int_distribution<std::int8_t> uniform{0, 3};

    for (hsize_t i = 0; i < shape[0]; i++) {
        for (hsize_t j = 0; j < shape[1]; j++) {
            hsize_t coord[] {i, j};
            H5Sselect_elements(filespace, H5S_SELECT_SET, 1, coord);

            auto value = uniform(random);
            H5Dwrite(dataset, datatype, memspace, filespace, H5P_DEFAULT, &value);

            // I expected that libhdf5 can convert integers to enums, but trying
            // to do so fails saying "can't find datatype conversion path".
            // H5Dwrite(dataset, H5T_NATIVE_INT8, memspace, filespace, H5P_DEFAULT, &value);
        }
    }
}
