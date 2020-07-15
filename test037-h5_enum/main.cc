#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <hdf5.h>


namespace h5 {
    template<typename T>
    hid_t memory_type() = delete;

    template<> inline hid_t memory_type<signed char>() { return H5T_NATIVE_SCHAR; }
    template<> inline hid_t memory_type<short>() { return H5T_NATIVE_SHORT; }
    template<> inline hid_t memory_type<int>() { return H5T_NATIVE_INT; }
    template<> inline hid_t memory_type<long>() { return H5T_NATIVE_LONG; }
    template<> inline hid_t memory_type<long long>() { return H5T_NATIVE_LLONG; }
    template<> inline hid_t memory_type<unsigned char>() { return H5T_NATIVE_UCHAR; }
    template<> inline hid_t memory_type<unsigned short>() { return H5T_NATIVE_USHORT; }
    template<> inline hid_t memory_type<unsigned int>() { return H5T_NATIVE_UINT; }
    template<> inline hid_t memory_type<unsigned long>() { return H5T_NATIVE_ULONG; }
    template<> inline hid_t memory_type<unsigned long long>() { return H5T_NATIVE_ULLONG; }
    template<> inline hid_t memory_type<float>() { return H5T_NATIVE_FLOAT; }
    template<> inline hid_t memory_type<double>() { return H5T_NATIVE_DOUBLE; }


    template<typename T>
    class enum_type
    {
    public:
        enum_type()
        {
            _type = H5Tenum_create(h5::memory_type<T>());
        }

        void insert(std::string const& name, T value)
        {
            H5Tenum_insert(_type, name.c_str(), &value);
        }

        hid_t type() const
        {
            return _type;
        }

    private:
        hid_t _type;
    };


    template<int rank, typename T>
    class rank_type
    {
    public:
        explicit rank_type(T const& type)
            : _type{type}
        {
        }

    private:
        T _type;
    };


    template<int rank, typename T>
    h5::rank_type<rank, T> dim(T const& type)
    {
        return h5::rank_type<rank, T>(type);
    }


    template<typename T>
    class dataset
    {
    public:
        dataset(hid_t file, std::string const& path, T const& type)
            : _file{file}, _path{path}, _type{type}
        {
        }

        template<typename B>
        void write(B const& buf)
        {
        }

    private:
        hid_t _file;
        std::string _path;
        T _type;
    };


    class file
    {
    public:
        explicit file(std::string const& filename)
        {
            _file = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
        }

        template<typename T>
        h5::dataset<T> dataset(std::string const& path, T const& type)
        {
            return h5::dataset<T>(_file, path, type);
        }

    private:
        hid_t _file = -1;
    };
}


int main()
{
    h5::file store("_output.h5");

    //auto ds = store.dataset<h5::enum_t<int>>("types");

    h5::enum_type<int> enum_type;
    enum_type.insert("A", 1);
    enum_type.insert("B", 2);
    enum_type.insert("C", 3);

    auto dataset = store.dataset("types", h5::dim<1>(enum_type));

    std::vector<int> data;
    data.push_back(1);
    data.push_back(2);
    data.push_back(3);

    dataset.write(data);
}
