# AtmTaskScheduling
Task Scheduling Rest API solution

This project is a HTTP server application written in C++ that demonstrates handling HTTP requests for managing a scheduling algorithm for different tasks of different nodes entities using the Boost ASIO, Beast libraries and libpqxx library, a C++ API to the PostgreSQL database management system.

## Features

## Prerequisites
* C++20 compatible compiler (GCC, Clang, MSVC, etc.)
* CMake version 3.14 or higher
* Boost libraries, version 1.71.0 or higher
* Libpqxx library 7.x versions

## Buiding the Project
1. Clone the repository:
```
git clone https://github.com/meli-mira/AtmTaskScheduling.git
cd AtmTaskScheduling
mkdir build
cd build
```

2. Configure the project with CMake:
```
cmake ..
make
```
This will generate an executable file in the build directory.
## Running the Server

Usage: TaskScheduling <server_address>  <port>  <threads>

Example:
```
./TaskScheduling 0.0.0.0 8080 1
```
## Available Endpoints


*
```
  GET /tasks
```
 get all the tasks

 * 0.2
    * Various bug fixes and optimizations
    * See [commit change]() or See [release history]()
* 0.1
    * Initial Release

