//
// Created by yizhou on 29/7/2023.
//
#include "config.hpp"
#include <array>
#include <nlohmann/json.hpp>

namespace doicom {

void to_json (nlohmann::json &json, const net_config_t &net_config) {
    json["control_check_status"] = net_config.control_check_status;
    json["adapter_num"] = net_config.adapter_num;
    json["ip_dog"] = net_config.ip_dog;
    json["auth_version"] = net_config.auth_version;
    json["keepalive_version"] = net_config.keepalive_version;
    json["ror_version"] = net_config.ror_version;
    json["keepalive_mode"] = net_config.keepalive_mode;
    json["pppoe_flag"] = net_config.pppoe_flag;
    json["keepalive2_flag"] = net_config.keepalive2_flag;
    json["eapol_flag"] = net_config.eapol_flag;
    json["eternal_flag"] = net_config.eternal_flag;
}

void from_json (const nlohmann::json &json, net_config_t &net_config) {
    json.at("control_check_status").get_to(net_config.control_check_status);
    json.at("adapter_num").get_to(net_config.adapter_num);
    json.at("ip_dog").get_to(net_config.ip_dog);
    json.at("auth_version").get_to(net_config.auth_version);
    json.at("keepalive_version").get_to(net_config.keepalive_version);
    json.at("ror_version").get_to(net_config.ror_version);
    json.at("keepalive_mode").get_to(net_config.keepalive_mode);
    json.at("pppoe_flag").get_to(net_config.pppoe_flag);
    json.at("keepalive2_flag").get_to(net_config.keepalive2_flag);
    json.at("eapol_flag").get_to(net_config.eapol_flag);
    json.at("eternal_flag").get_to(net_config.eternal_flag);
}

void to_json (nlohmann::json &json, const user_config_t &user_config) {
    json["auth_server"] = user_config.auth_server;
    json["account"] = user_config.account;
    json["password"] = user_config.password;
    json["host_ip"] = user_config.host_ip;
    json["host_name"] = user_config.host_name;
    json["primary_dns"] = user_config.primary_dns;
    json["dhcp_server"] = user_config.dhcp_server;
    json["mac_addr"] = user_config.mac_addr;
    json["platform"] = user_config.platform;
}

void from_json (const nlohmann::json &json, user_config_t &user_config) {
     json.at("auth_server").get_to(user_config.auth_server);
     json.at("account").get_to(user_config.account);
     json.at("password").get_to(user_config.password);
     json.at("host_ip").get_to(user_config.host_ip);
     json.at("host_name").get_to(user_config.host_name);
     json.at("primary_dns").get_to(user_config.primary_dns);
     json.at("dhcp_server").get_to(user_config.dhcp_server);
     json.at("mac_addr").get_to(user_config.mac_addr);
     json.at("platform").get_to(user_config.platform);
}

}