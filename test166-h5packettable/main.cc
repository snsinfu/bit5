#include <cstdint>

#include <hdf5.h>
#include <hdf5_hl.h>


using std::int32_t;


int main()
{
    constexpr hsize_t chunk_size = 32;

    hid_t file = H5Fcreate("_output.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    // A record object represents a single record in packet table.
    struct record
    {
        int32_t state;
        float x;
        float y;
    };
    static_assert(sizeof(record) == 12);

    hid_t record_dtype = H5Tcreate(H5T_COMPOUND, 12);

    H5Tinsert(record_dtype, "state", 0, H5T_STD_I32LE);
    H5Tinsert(record_dtype, "x", 4, H5T_IEEE_F32LE);
    H5Tinsert(record_dtype, "y", 8, H5T_IEEE_F32LE);

    hid_t pt = H5PTcreate(
        file,
        "stream",
        record_dtype,
        chunk_size,
        H5P_DEFAULT
    );

    for (int i = 0; i < 10; i++) {
        record const rec = {
            .state = int32_t((i + 1) * (i + 2)),
            .x = 1.2F * float(i),
            .y = 3.4F
        };
        H5PTappend(pt, 1, &rec);
    }

    H5Tclose(record_dtype);
    H5PTclose(pt);
    H5Fclose(file);
}
