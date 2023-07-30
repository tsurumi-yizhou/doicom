//
// Created by yizhou on 29/7/2023.
//

#ifndef DOICOM_LOGGING_HPP
#define DOICOM_LOGGING_HPP
#include <string>
#include <fmt/color.h>

namespace doicom {

inline std::string now ();

void info (const std::string &thing);

void panic (const std::string &error);

}

#endif //DOICOM_LOGGING_HPP
