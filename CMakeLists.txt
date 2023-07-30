cmake_minimum_required(VERSION 3.27)
project(doicom)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
file (GLOB_RECURSE SRCS source/*.cpp)
list(REMOVE_ITEM ${SRCS} source/main.cpp)
add_executable(doicom source/main.cpp ${SRCS})

add_subdirectory(test)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(doicom PRIVATE fmt::fmt-header-only)

find_package(range-v3 CONFIG REQUIRED)
target_link_libraries(doicom PRIVATE range-v3::meta range-v3::concepts range-v3::range-v3)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(doicom PRIVATE nlohmann_json::nlohmann_json)

find_package(Boost REQUIRED COMPONENTS system)
target_link_libraries(doicom PRIVATE Boost::boost Boost::system)

