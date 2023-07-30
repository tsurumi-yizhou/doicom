//
// Created by yizhou on 30/7/2023.
//

#ifndef DOICOM_KEEPALIVE_HPP
#define DOICOM_KEEPALIVE_HPP

#include <array>
#include <optional>
#include <span>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/ip/udp.hpp>
#include "config.hpp"

namespace net = boost::asio;
using net::ip::udp;

namespace doicom {

net::awaitable<void> keepalive1 (std::shared_ptr<udp::socket> socket, udp::endpoint endpoint, uint32_t keepalive_mode,
                                 const std::array<uint8_t, 16> &auth_info, const std::array<uint8_t, 4> &seed,
                                 const std::string &password);

net::awaitable<void>
keepalive2 (std::shared_ptr<udp::socket> socket, udp::endpoint endpoint, const std::array<uint8_t, 2>& keepalive_version,
            const std::array<uint8_t, 4> &host_ip, MODE mode);

}

#endif //DOICOM_KEEPALIVE_HPP
