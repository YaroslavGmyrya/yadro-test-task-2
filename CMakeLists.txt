cmake_minimum_required(VERSION 4.2.3)
project(main LANGUAGES CXX)

# find spdlog
find_package(spdlog REQUIRED)

set(SRC 
    cpp/src/config_loader.cpp
    cpp/src/sub_funcs.cpp
)

set(TESTS_SRC
    tests/config_loader_test.cpp
    tests/main_test.cpp
)

# add_executable(main $(SRC))
add_executable(tests ${TESTS_SRC} ${SRC})

# target_link_libraries(main PRIVATE spdlog::spdlog)
target_link_libraries(tests PRIVATE spdlog::spdlog)