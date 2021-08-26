#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

#include <jsoncons/json.hpp>

#include "assert.hpp"


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


int main()
{
    using namespace std::string_literals;

    // null is decoded as nullopt.
    auto const config_1 = jsoncons::decode_json<server_config>(R"(
        {
            "hostname": "127.0.0.1",
            "log_file": null
        }
    )"s);
    TRACE_ASSERT(!config_1.log_file);

    // optional field is filled by value
    auto const config_2 = jsoncons::decode_json<server_config>(R"(
        {
            "log_file": "logs/server.log"
        }
    )"s);
    TRACE_ASSERT(config_2.log_file);
    TRACE_ASSERT(*config_2.log_file == "logs/server.log");
}
