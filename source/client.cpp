//
// Created by yizhou on 29/7/2023.
//
#include "client.hpp"
#include "dhcp.hpp"
#include "keepalive.hpp"
#include "logging.hpp"

doicom::client_t::client_t (net::io_context &context, net_config_t &net, user_config_t &user)
        : net_config(net), user_config(user), endpoint({net::ip::address_v4::from_string(user.auth_server), 61440}) {
    socket = std::make_shared<udp::socket>(context);
}

net::awaitable<void> doicom::client_t::challenge () {
    auto seed_result = co_await doicom::challenge(socket, endpoint, net_config.auth_version);
    if (!seed_result.has_value()) panic("can't read seed from challenge response");
    seed = seed_result.value();
}

net::awaitable<void> doicom::client_t::login () {
    auto auth_info_result = co_await doicom::login(socket, endpoint, seed,
                                                                                  user_config.account,
                                                                                  user_config.password,
                                                                                  user_config.mac_addr,
                                                                                  user_config.host_ip,
                                                                                  user_config.primary_dns,
                                                                                  user_config.dhcp_server,
                                                                                  user_config.host_name,
                                                                                  user_config.platform,
                                                                                  net_config.ror_version,
                                                                                  net_config.control_check_status,
                                                                                  net_config.adapter_num,
                                                                                  net_config.ip_dog,
                                                                                  net_config.auth_version);
    if (! auth_info_result.has_value()) panic("can't read auth info from challenge response");
    auth_info = auth_info_result.value();
}

net::awaitable<void> doicom::client_t::keepalive () {
    co_await doicom::keepalive1(socket, endpoint, net_config.keepalive_mode, auth_info, seed, user_config.password);
    co_await doicom::keepalive2(socket, endpoint, net_config.keepalive_version, user_config.host_ip, MODE::DHCP);
}

