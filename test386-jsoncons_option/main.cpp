#include <iostream>
#include <optional>
#include <string>

#include <jsoncons/json.hpp>


struct server_config
{
    std::string                hostname = "0.0.0.0";
    unsigned                   port     = 8080;
    std::optional<std::string> log_file;
};

JSONCONS_N_MEMBER_TRAITS(
    server_config,

    // Required fields
    0,

    // Optional fields
    hostname,
    port,
    log_file
)


void examine_config(server_config const& config)
{
    std::cout << "=== config ===\n";

    std::cout << "hostname: " << config.hostname << '\n';
    std::cout << "port: " << config.port << '\n';

    if (config.log_file) {
        std::cout << "log file: " << *config.log_file << '\n';
    } else {
        std::cout << "no log file\n";
    }
}


int main()
{
    using namespace std::string_literals;

    auto const config_1 = jsoncons::decode_json<server_config>(R"(
        {
            "hostname": "127.0.0.1"
        }
    )"s);

    auto const config_2 = jsoncons::decode_json<server_config>(R"(
        {
            "log_file": "logs/server.log"
        }
    )"s);

    examine_config(config_1);
    examine_config(config_2);
}
