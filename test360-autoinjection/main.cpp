#include <functional>
#include <vector>

#include <md.hpp>


class injector
{
public:
    template<typename T, typename Tag>
    void register_attribute(md::attribute_key<T, Tag> key)
    {
        injectors.push_back([=](md::system& system) { system.require(key); });
    }

    void inject(md::system& system)
    {
        for (auto& injector : injectors) {
            injector(system);
        }
    }

    static injector& main()
    {
        static injector instance;
        return instance;
    }

private:
    std::vector<std::function<void(md::system&)>> injectors;
};


template<typename T, typename Tag = T>
struct auto_attribute
{
    using key_type = md::attribute_key<T, Tag>;

    key_type key;

    auto_attribute(key_type key = {})
        : key(key)
    {
        injector::main().register_attribute(key);
    }

    operator key_type() const
    {
        return key;
    }

    md::array_view<T> operator()(md::system& system) const
    {
        return system.view(key);
    }

    md::array_view<T const> operator()(md::system const& system) const
    {
        return system.view(key);
    }
};


struct particle_data
{
    int id = 0;
};

auto_attribute<particle_data> particle_data_attribute;


int main()
{
    md::system system;

    injector::main().inject(system);

    system.add_particle();
    system.add_particle();
    system.add_particle();

    // `system.view(particle_data_attribute)` does not work. Template parameter
    // deduction interferes with the conversion operator on auto_attribute.
    auto data = particle_data_attribute(system);

    data[0].id = 0;
    data[1].id = 1;
    data[2].id = 2;
}
