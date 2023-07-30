//
// Created by yizhou on 30/7/2023.
//
#include "keepalive.hpp"

net::awaitable<void> doicom::keepalive1 (std::shared_ptr<udp::socket> socket, udp::endpoint endpoint, uint32_t keepalive_mode,
                                 const std::array<uint8_t, 16> &auth_info, const std::array<uint8_t, 4> &seed,
                                 const std::string &password){}

net::awaitable<void>
doicom::keepalive2 (std::shared_ptr<udp::socket> socket, udp::endpoint endpoint, const std::array<uint8_t, 2>& keepalive_version,
            const std::array<uint8_t, 4> &host_ip, MODE mode){}