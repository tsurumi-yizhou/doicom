file(GLOB TESTS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(tester ${TESTS} ${SRCS})

find_package(Boost REQUIRED COMPONENTS unit_test_framework)
target_link_libraries(tester PRIVATE Boost::boost Boost::unit_test_framework)