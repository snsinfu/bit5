#include <iostream>
#include <string>
#include <vector>

#include <jsoncons/json.hpp>


struct site_data
{
    int    start  = 0;
    int    end    = 0;
    double weight = 0;
};

struct config_type
{
    std::vector<site_data> sites;
};

JSONCONS_ALL_MEMBER_TRAITS(
    config_type,
    sites
)

struct site_data_proxy : site_data
{
    std::optional<int> site;
};

JSONCONS_N_MEMBER_TRAITS(
    site_data_proxy,

    1,
    weight,

    start,
    end,
    site
)


template<class Json>
struct jsoncons::json_type_traits<Json, site_data>
{
    using value_type = site_data;
    using proxy_type = site_data_proxy;
    using proxy_traits = jsoncons::json_type_traits<Json, proxy_type>;
    using allocator_type = typename Json::allocator_type;

    static bool is(const Json& j) noexcept
    {
        return proxy_traits::is(j);
    }

    static value_type as(const Json& j)
    {
        auto proxy = proxy_traits::as(j);
        if (proxy.site) {
            proxy.start = *proxy.site;
            proxy.end = proxy.start + 1;
        }
        return static_cast<value_type>(proxy);
    }

    static Json to_json(
        value_type const& value,
        allocator_type    alloc = allocator_type{}
    )
    {
        proxy_type proxy;
        static_cast<value_type&>(proxy) = value;
        return proxy_traits::to_json(proxy, alloc);
    }
};


int main()
{
    std::string const source = R"(
        {
            "sites": [
                {"site": 3, "weight": 7},
                {"site": 5, "weight": 10},
                {"start": 0, "end": 10, "weight": 1}
            ]
        }
    )";

    auto const config = jsoncons::decode_json<config_type>(source);

    for (auto const& site : config.sites) {
        std::string text;
        jsoncons::encode_json(site, text);
        std::cout << text << '\n';
    }
}
