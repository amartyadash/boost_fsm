cmake_minimum_required(VERSION 3.0.0)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(stopwatch_sc stopwatch_sc.cpp)
target_link_libraries(stopwatch_sc LINK_PUBLIC ${Boost_LIBRARIES})