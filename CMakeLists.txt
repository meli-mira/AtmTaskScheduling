cmake_minimum_required(VERSION 3.10)
project(TaskScheduling)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS system filesystem json)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")

find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

file(GLOB SOURCES "src/*.cpp" "src/*/*.cpp")
add_executable(TaskScheduling ${SOURCES})

target_include_directories(TaskScheduling PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(TaskScheduling ${PQXX_LIB} ${PQ_LIB} ${Boost_LIBRARIES})
