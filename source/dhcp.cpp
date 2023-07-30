//
// Created by yizhou on 29/7/2023.
//
#include "dhcp.hpp"
#include <random>
#include <boost/asio/use_awaitable.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>
#include "logging.hpp"

namespace doicom {

std::string md5(std::span<uint8_t> data) noexcept {
    boost::uuids::detail::md5 generator;
    generator.process_bytes(data.data(), data.size());
    boost::uuids::detail::md5::digest_type digest;
    generator.get_digest(digest);
    const auto char_digest = reinterpret_cast<char*>(&digest);
    std::string result;
    boost::algorithm::hex(char_digest, char_digest + sizeof(digest), std::back_inserter(result));
    return result;
}

net::awaitable<std::optional<std::array<uint8_t, 4>>> challenge (std::shared_ptr<udp::socket> socket, udp::endpoint endpoint, std::array<uint8_t, 2> auth_version) noexcept {
    try {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<uint8_t> dist(0x00, 0xff);
        std::vector<uint8_t> send_buffer {0x01, 0x02, dist(mt), dist(mt), auth_version[0]};
        send_buffer.resize(20);
        co_await socket->async_send_to(net::buffer(send_buffer), endpoint, net::use_awaitable);

        std::vector<uint8_t> recv_buffer(1024, 0);
        co_await socket->async_receive_from(net::buffer(recv_buffer), endpoint, net::use_awaitable);
        assert(recv_buffer[0] == 0x02);
        std::array<uint8_t, 4> seed{};
        std::copy(recv_buffer.begin() + 4, recv_buffer.begin() + 7, seed.begin());
        co_return seed;
    } catch (...) {
        co_return std::nullopt;
    }
}

net::awaitable<std::optional<std::array<uint8_t, 16>>>
login (std::shared_ptr<udp::socket> socket, udp::endpoint endpoint, const std::array<uint8_t, 4> &seed, const std::string &account, const std::string &password, const std::array<uint8_t, 6> &mac_addr,
       const std::array<uint8_t, 4> &host_ip, const std::array<uint8_t, 4> &dns, const std::array<uint8_t, 4> &dhcp_server, const std::string &host_name, const std::string &platform,
       uint8_t ror_version, uint8_t control_check_status, uint8_t adapter_num,
       uint8_t ip_dog, std::array<uint8_t, 2> auth_version) noexcept {
    assert(password.size() > 8);
    assert(host_ip.size() == 4);
    assert(mac_addr.size() == 6);
    assert(dns.size() == 4);
    assert(dhcp_server.size() == 4);
    std::size_t length_padding = 20 + password.size() + ((password.size() == 16) ? 0 : password.size() >> 2);
    std::size_t packet_size = ror_version ? 338 + length_padding : 330;
    std::vector<uint8_t> send_buffer {0x03, 0x01, 0x00, (uint8_t)(account.size() + 20)};  // size: 4
    send_buffer.reserve(packet_size);

    std::vector<uint8_t> md5_buffer1 {0x03, 0x01, seed[0], seed[1], seed[2], seed[3]};
    md5_buffer1.insert(md5_buffer1.end(), password.begin(), password.end());
    auto md5_value1 = md5(md5_buffer1);
    send_buffer.insert(send_buffer.end(), md5_value1.begin(), md5_value1.end());  // size: 20

    send_buffer.insert(send_buffer.end(), account.begin(), account.end());  // size: 56

    send_buffer.resize(56, 0);
    send_buffer.push_back(control_check_status);  // size: 57
    send_buffer.push_back(adapter_num);  // size: 58

    uint64_t sum = 0, mac = 0;
    std::vector<uint8_t> xor_md5 (6, 0);
    for(auto i = 0; i < 6; i ++) {
        sum = (int) md5_value1[i] + (sum << 8);
    }
    for (auto i = 0; i < 6; i ++) {
        mac = (int) mac_addr[i] + (mac << 8);
    }
    sum ^= mac;
    for (auto i = 6; i > 0; i --) {
        xor_md5[i - 1] = (uint8_t) (sum % 256);
        sum = (sum >> 8);
    }
    send_buffer.insert(send_buffer.end(), xor_md5.begin(), xor_md5.end());  // size: 64

    std::vector<uint8_t> md5_buffer2 {0x01};
    md5_buffer2.insert(md5_buffer2.end(), password.begin(), password.end());
    md5_buffer2.insert(md5_buffer2.end(), seed.begin(), seed.end());
    auto md5_value2 = md5(md5_buffer2);
    send_buffer.insert(send_buffer.end(), md5_value2.begin(), md5_value2.end());  // size: 80

    send_buffer.push_back(0x01);  // size: 81
    assert(host_ip.size() == 4);
    send_buffer.insert(send_buffer.end(), host_ip.begin(), host_ip.end());  // size: 85
    std::vector<uint8_t> checksum1 (send_buffer.begin(), send_buffer.end());
    checksum1.resize(97, 0);
    checksum1.insert(checksum1.end(), {0x14, 0x00, 0x07, 0x0b});
    auto checksum1_md5 = md5(checksum1);
    send_buffer.resize(97);   // size: 97
    send_buffer.insert(send_buffer.end(), checksum1_md5.begin(), checksum1_md5.end()); // size: 105
    send_buffer.resize(105);  // size: 105
    send_buffer.push_back(ip_dog);    // size: 106
    send_buffer.resize(110);  // size: 110
    send_buffer.insert(send_buffer.end(), host_name.begin(), host_name.end());  // size: 142
    send_buffer.resize(142);  // size: 142
    send_buffer.insert(send_buffer.end(), dns.begin(), dns.end());  // size: 146
    send_buffer.resize(146);  // size: 146
    send_buffer.insert(send_buffer.end(), dhcp_server.begin(), dhcp_server.end());  // size: 150
    send_buffer.resize(150);  // size: 150
    send_buffer.resize(162);
    send_buffer.insert(send_buffer.end(), {0x94, 0x00, 0x00, 0x00});    // size: 166
    send_buffer.insert(send_buffer.end(), {0x06, 0x00, 0x00, 0x00});    // size: 170
    send_buffer.insert(send_buffer.end(), {0x02, 0x00, 0x00, 0x00});    // size: 174
    send_buffer.insert(send_buffer.end(), {0xf0, 0x23, 0x00, 0x00});    // size: 178
    send_buffer.insert(send_buffer.end(), {0x02, 0x00, 0x00, 0x00});    // size: 182
    send_buffer.insert(send_buffer.end(), {0x44, 0x72, 0x43, 0x4f, 0x4d, 0x00, 0xcf, 0x07, 0x68});  // size: 192
    send_buffer.resize(246);
    send_buffer.insert(send_buffer.end(), {0x33, 0x64, 0x63, 0x37, 0x39, 0x66, 0x35, 0x32, 0x31, 0x32, 0x65, 0x38, 0x31,
                                               0x37, 0x30, 0x61, 0x63, 0x66, 0x61, 0x39, 0x65, 0x63, 0x39, 0x35, 0x66, 0x31,
                                               0x64, 0x37, 0x34, 0x39, 0x31, 0x36, 0x35, 0x34, 0x32, 0x62, 0x65, 0x37, 0x62,
                                               0x31});      // size: 266
    send_buffer.resize(310);
    send_buffer.insert(send_buffer.end(), {auth_version[0], auth_version[1]});  // size: 312
    size_t ror_padding = (password.size() <= 8) ? 8 - password.size() : ((password.size() == 16) ? 0 : password.size() >> 2);
    size_t counter = 312;
    if (ror_version) {
        send_buffer.insert(send_buffer.end(), (uint8_t)password.size());
        auto x = 0;
        for (auto i = 0; i < password.size(); i ++) {
            x = (int) md5_value1[i] ^ (int) password[i];
            send_buffer.insert(send_buffer.end(), ((x << 3) & 0xff) + (x >> 5) );
        }
        counter += 2 + password.size();
    } else {
        ror_padding = 2;
    }
    send_buffer.insert(send_buffer.end(), {0x02, 0x0c});
    std::vector<uint8_t> checksum2_buffer;
    checksum2_buffer.insert(checksum2_buffer.end(), send_buffer.begin(), send_buffer.begin() + counter + 2);
    checksum2_buffer.insert(checksum2_buffer.end(), {0x01, 0x26, 0x07, 0x11, 0x00, 0x00});
    checksum2_buffer.insert(checksum2_buffer.end(), mac_addr.begin(), mac_addr.end());
    sum = 1234;
    uint64_t ret = 0;
    for (auto i = 0; i < counter + 14; i += 4) {
        ret = 0;
        for (auto j = 4; j > 0; j --) {
            ret = (ret << 8) + (int) checksum2_buffer[i + j - 1];
        }
        sum ^= ret;
    }
    sum = (1968 * sum) & 0xfffffffff;
    std::vector<uint8_t> checksum2 (4, 0);
    for (auto j = 0; j < 4; j ++) {
        checksum2[j] = (uint8_t) (sum >> (j * 8) & 0xff);
    }
    send_buffer.insert(send_buffer.end(), checksum2.begin(), checksum2.end());
    send_buffer.insert(send_buffer.end(), mac_addr.begin(), mac_addr.end());
    send_buffer.insert(send_buffer.end(), {0x60, 0xa2});

    co_await socket->async_send_to(net::buffer(send_buffer), endpoint, net::use_awaitable);
    std::vector<uint8_t> recv_buffer(1024, 0);
    co_await socket->async_receive_from(net::buffer(recv_buffer), endpoint, net::use_awaitable);
    if (recv_buffer[0] != 0x04) {
        info(fmt::format("recv_buffer[0]={} & recv_buffer[4]={}\n", recv_buffer[0], recv_buffer[4]));
        co_return std::nullopt;
    }
    std::array<uint8_t, 16> result {};
    std::copy(recv_buffer.begin() + 23, recv_buffer.begin() + 39, result.begin());
    co_return result;
}

}