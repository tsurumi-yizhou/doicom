//
// Created by yizhou on 29/7/2023.
//

#ifndef DOICOM_DHCP_HPP
#define DOICOM_DHCP_HPP
#include <array>
#include <optional>
#include <span>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/ip/udp.hpp>

namespace net = boost::asio;
using net::ip::udp;

namespace doicom {

std::string md5(std::span<uint8_t> data) noexcept;
net::awaitable<std::optional<std::array<uint8_t, 4>>> challenge (std::shared_ptr<udp::socket> socket, udp::endpoint endpoint, std::array<uint8_t, 2> auth_version) noexcept;
net::awaitable<std::optional<std::array<uint8_t, 16>>>
login (std::shared_ptr<udp::socket> socket, udp::endpoint endpoint, const std::array<uint8_t, 4> &seed, const std::string &account, const std::string &password, const std::array<uint8_t, 6> &mac_addr,
       const std::array<uint8_t, 4> &host_ip, const std::array<uint8_t, 4> &dns, const std::array<uint8_t, 4> &dhcp_server, const std::string &host_name, const std::string &platform,
       uint8_t ror_version, uint8_t control_check_status, uint8_t adapter_num,
       uint8_t ip_dog, std::array<uint8_t, 2> auth_version) noexcept;

}

#endif //DOICOM_DHCP_HPP
