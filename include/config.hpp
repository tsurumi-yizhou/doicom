//
// Created by yizhou on 29/7/2023.
//

#ifndef DOICOM_CONFIG_HPP
#define DOICOM_CONFIG_HPP
#include <string>
#include <nlohmann/json.hpp>

namespace doicom {

enum class MODE {
    DHCP, PPPOE
};

struct net_config_t {
    uint8_t control_check_status;
    uint8_t adapter_num;
    uint8_t ip_dog;
    std::array<uint8_t, 2> auth_version;
    std::array<uint8_t, 2> keepalive_version;
    uint32_t ror_version;
    uint32_t keepalive_mode;
    uint8_t pppoe_flag;
    uint8_t keepalive2_flag;
    uint32_t eapol_flag;
    uint32_t eternal_flag;
};

void to_json(nlohmann::json& json, const net_config_t& net_config);
void from_json(const nlohmann::json& json, net_config_t& net_config);

struct user_config_t {
    std::string auth_server;
    std::string account;
    std::string password;
    std::array<uint8_t, 4> host_ip;
    std::string host_name;
    std::array<uint8_t, 4> primary_dns;
    std::array<uint8_t, 4> dhcp_server;
    std::array<uint8_t, 6> mac_addr;
    std::string platform;
};

void to_json (nlohmann::json &json, const user_config_t &user_config);
void from_json(const nlohmann::json& json, user_config_t& user_config);

}

#endif //DOICOM_CONFIG_HPP
