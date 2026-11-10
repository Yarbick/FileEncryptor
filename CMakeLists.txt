cmake_minimum_required(VERSION 4.2)
project(FyleEncryptor)

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

add_executable(FyleEncryptor ${SOURCES})

target_link_libraries(FyleEncryptor PRIVATE Qt6::Widgets)
