cmake_minimum_required(VERSION 3.2)

project(tVM
	LANGUAGES CXX
	VERSION 0.0.1
)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(tvm)
add_subdirectory(tvm-run)
