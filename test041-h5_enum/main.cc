#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include <hdf5.h>


namespace h5
{
    /*
     * An `enums` holds member definitions of an enumerated type.
     */
    template<typename T>
    class enums
    {
    public:
        /*
         * Pair of enum member name and value.
         */
        struct member
        {
            std::string name;
            T value;
        };

        /*
         * Iterator over enumerated members.
         */
        using iterator = typename std::vector<member>::const_iterator;

        /*
         * Default constructor creates an empty enum.
         */
        enums() = default;

        /*
         * Creates an enum containing given members.
         */
        enums(std::initializer_list<member> members)
            : _members(members)
        {
        }

        /*
         * Returns the number of members in this enum.
         */
        std::size_t size() const
        {
            return _members.size();
        }

        /*
         * Returns the start of a range containing enum members.
         */
        iterator begin() const
        {
            return _members.begin();
        }

        /*
         * Returns the end of a range containing enum members.
         */
        iterator end() const
        {
            return _members.end();
        }

        /*
         * Finds a pointer to the value associated to given name. Returns null
         * if not found.
         *
         * XXX: Is it really reasonable to use pointer as optional here?
         */
        T const* value(std::string const& query_name) const
        {
            for (auto const& [ name, value ] : _members) {
                if (name == query_name) {
                    return &value;
                }
            }
            return nullptr;
        }

        /*
         * Finds a pointer to the name associated to given value. Returns null
         * if not found.
         */
        std::string const* name(T query_value) const
        {
            for (auto const& [ name, value ] : _members) {
                if (value == query_value) {
                    return &name;
                }
            }
            return nullptr;
        }

        /*
         * Inserts an enum member.
         */
        void insert(std::string const& name, T value)
        {
            _members.push_back({ name, value });
        }

    private:
        std::vector<member> _members;
    };


    /*
     * Returns native type HID for static type.
     */
    template<typename T>
    hid_t memory_type() = delete;

    template<> inline hid_t memory_type<int>() { return H5T_NATIVE_INT; }


    /*
     * Creates a new enumerated type baed on member definitions.
     */
    template<typename T>
    hid_t make_type(h5::enums<T> const& enums)
    {
        hid_t type = H5Tenum_create(h5::memory_type<T>());

        for (auto const& [ name, value ] : enums) {
            H5Tenum_insert(type, name.c_str(), &value);
        }

        return type;
    }


    /*
     * Validates enumeration type against enum definition.
     */
    template<typename T>
    bool validate_type(hid_t type, h5::enums<T> const& enums)
    {
        auto const members = H5Tget_nmembers(type);
        if (members < 0) {
            throw std::runtime_error("H5Tget_nmembers failed");
            return 0;
        }

        if (enums.size() != std::size_t(members)) {
            return false;
        }

        for (auto const& [ name, expected_value ] : enums) {
            T actual_value;
            if (H5Tenum_valueof(type, name.c_str(), &actual_value) < 0) {
                throw std::runtime_error("H5Tenum_valueof failed");
            }

            if (actual_value != expected_value) {
                return false;
            }
        }

        return true;
    }
}


int main()
{
    h5::enums<int> const enums = {
        {"A", 1}, {"B", 2}, {"C", 3}
    };

    assert(enums.size() == 3);

    assert(!enums.name(0));
    assert(*enums.name(1) == "A");
    assert(*enums.name(2) == "B");
    assert(*enums.name(3) == "C");
    assert(!enums.name(4));

    assert(*enums.value("A") == 1);
    assert(*enums.value("B") == 2);
    assert(*enums.value("C") == 3);
    assert(!enums.value("D"));

    for (auto const& [ name, value ] : enums) {
        std::clog << name << '\t' << value << '\n';
    }

    // Use make_type when writing enum dataset to file.
    auto type = h5::make_type(enums);

    // Use validate_type when reading enum dataset from file.
    if (h5::validate_type(type, enums)) {
        std::clog << "valid\n";
    } else {
        std::clog << "not valid\n";
    }

    H5Tclose(type);
}
