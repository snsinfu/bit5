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


template<class Json>
struct jsoncons::json_type_traits<Json, site_data>
{
    using allocator_type = typename Json::allocator_type;

    static bool is(const Json& j) noexcept
    {
        return j.is_object() && (
            j.contains("site") || (j.contains("start") && j.contains("end"))
        );
    }

    static site_data as(const Json& j)
    {
        site_data value;
        if (j.contains("site")) {
            value.start = j.at("site").template as<int>();
            value.end = value.start + 1;
        } else {
            value.start = j.at("start").template as<int>();
            value.end = j.at("end").template as<int>();
        }
        value.weight = j.at("weight").template as<double>();
        return value;
    }

    static Json to_json(
        site_data const& value,
        allocator_type   alloc = allocator_type()
    )
    {
        Json j{jsoncons::json_object_arg_t{}, jsoncons::semantic_tag::none, alloc};
        j.try_emplace("start", value.start);
        j.try_emplace("end", value.end);
        j.try_emplace("weight", value.weight);
        return j;
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
