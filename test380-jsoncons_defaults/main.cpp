#include <iostream>
#include <string>

#include <jsoncons/json.hpp>


struct server_config
{
    std::string hostname = "";
    unsigned    port     = 8080;
    std::string root     = "/var/www";
};

JSONCONS_N_MEMBER_TRAITS(
    server_config,

    // Required fields
    0,

    // Optional fields
    hostname,
    port,
    root
)


int main()
{
    // Struct field default is honored. `port` is not specified in the JSON,
    // so the default value 8080 would be used.

    std::string const config_json = R"(
        {
            "hostname": "127.0.0.1",
            "root":     "/srv/www"
        }
    )";
    auto const config = jsoncons::decode_json<server_config>(config_json);

    std::cout << config.hostname << ':' << config.port << '\n';
    std::cout << config.root << '\n';
}
