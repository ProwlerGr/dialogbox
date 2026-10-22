cmake_minimum_required(VERSION 3.16)
project(dialogbox LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(dialogbox
    src/dialogbox.cc
    src/dialog_parser.cc
    src/dialog_main.cc
    src/dialog_set_options.cc
    src/dialog_slots.cc
    src/dialog_private.cc
)

target_include_directories(dialogbox PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(dialogbox PRIVATE Qt6::Widgets)
