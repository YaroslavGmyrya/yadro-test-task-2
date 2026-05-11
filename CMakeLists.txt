cmake_minimum_required(VERSION 4.2.3)
project(main LANGUAGES CXX)

# find spdlog
find_package(spdlog REQUIRED)

set(SRC 
    cpp/src/config_loader.cpp
    cpp/src/sub_funcs.cpp
    cpp/src/traffic_models.cpp
)

set(TESTS_SRC
    tests/config_loader_test.cpp
    tests/main_test.cpp
    tests/main_test.cpp
)

add_executable(main ${SRC} cpp/src/main.cpp)
add_executable(tests ${TESTS_SRC} ${SRC})

target_link_libraries(main PRIVATE spdlog::spdlog)
target_link_libraries(tests PRIVATE spdlog::spdlog)