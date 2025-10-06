cmake_minimum_required(VERSION 3.16)
project(SysMood VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Put sources in a target
add_executable(SysMood
    src/main.cpp
)

target_include_directories(SysMood PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Enable testing (used later)
include(CTest)
enable_testing()

# (Optional) install target
install(TARGETS SysMood RUNTIME DESTINATION bin)
