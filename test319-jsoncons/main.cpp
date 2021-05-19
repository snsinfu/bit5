#include <cstdint>
#include <iostream>
#include <string>

#include <jsoncons/json.hpp>


struct app_config
{
    std::string              hostname;
    std::uint16_t            port;
    std::uint32_t            threads;
    std::vector<std::string> resource_paths;
};

JSONCONS_ALL_MEMBER_TRAITS(
    app_config,
    hostname,
    port,
    threads,
    resource_paths
)

int main()
{
    std::string const source = R"(
        {
            "hostname":       "example.com",
            "port":           3000,
            "threads":        64,
            "resource_paths": ["/srv/resources", "./resources"],

            "__comment": [
                "This is a dummy comment field"
            ]
        }
    )";

    auto const config = jsoncons::decode_json<app_config>(source);

    std::cout
        << "hostname: " << config.hostname << '\n'
        << "port:     " << config.port     << '\n'
        << "threads:  " << config.threads  << '\n';

    std::cout << "resource paths:\n";

    for (auto const& path : config.resource_paths) {
        std::cout << "  '" << path << "'\n";
    }

    std::cout << "---\n";

    jsoncons::encode_json_pretty(config, std::cout);

    std::cout << '\n';
}
