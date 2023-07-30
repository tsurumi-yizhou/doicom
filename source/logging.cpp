//
// Created by yizhou on 29/7/2023.
//
#include "logging.hpp"
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>

std::string doicom::now () {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm *local_now = std::localtime(&now_c);
    std::ostringstream ss;
    ss << std::put_time(local_now, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void doicom::info (const std::string &thing) {
    fmt::print(fmt::fg(fmt::color::green), "{} [INFO]: {}\n", now(), thing);
}

void doicom::panic (const std::string &error) {
    fmt::print(fmt::fg(fmt::color::red), "{} [PANIC]: {}\n", now(), error);
}