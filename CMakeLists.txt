cmake_minimum_required(VERSION 3.7)
project(annotated_tfs)

set(CMAKE_CXX_STANDARD 11)

include_directories(src import)

file(GLOB import_src "import/*.cpp")
file(GLOB_RECURSE src_src "src/*.cpp")

add_executable(annotated_tfs ${import_src} ${src_src})