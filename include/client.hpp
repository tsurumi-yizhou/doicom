//
// Created by yizhou on 29/7/2023.
//

#ifndef DOICOM_CLIENT_HPP
#define DOICOM_CLIENT_HPP
#include <boost/asio/io_context.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/ip/udp.hpp>
#include "config.hpp"

namespace net = boost::asio;
using net::ip::udp;

namespace doicom {

struct client_t final {
private:
    std::shared_ptr<udp::socket> socket;
    udp::endpoint endpoint;
    net_config_t& net_config;
    user_config_t& user_config;
    std::array<uint8_t, 4> seed;
    std::array<uint8_t, 16> auth_info;

public:
    client_t (net::io_context &context, net_config_t& net, user_config_t& user);
    net::awaitable<void> challenge();
    net::awaitable<void> login();
    net::awaitable<void> keepalive();
};

}
#endif //DOICOM_CLIENT_HPP
