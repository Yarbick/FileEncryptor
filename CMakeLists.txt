cmake_minimum_required(VERSION 4.2)
project(FileEncryptor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

file(GLOB_RECURSE SOURCES
        "src/*.cpp"
        "src/*.h"
        "src/*.ui"
)

add_executable(FileEncryptor ${SOURCES})

target_include_directories(FileEncryptor PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_link_libraries(FileEncryptor PRIVATE Qt6::Widgets)
