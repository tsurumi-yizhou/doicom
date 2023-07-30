#include <fstream>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include "client.hpp"

net::awaitable<void> co_main(doicom::client_t& client) {
    co_await client.challenge();
    co_await client.login();
}

int main () {
    net::io_context context{1};
    std::ifstream reader("config.json");
    nlohmann::json config;
    reader >> config;
    reader.close();
    doicom::net_config_t net_config = config["network"];
    doicom::user_config_t user_config = config["user"];
    doicom::client_t client(context, net_config, user_config);
    net::co_spawn(context, co_main(client), net::detached);
    context.run();
    return 0;
}