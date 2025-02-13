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

* Tasks
    * GET /tasks : Retrives all the tasks available.
    * GET /tasks/{node_id} : Retrives all the tasks of node with node_id.
    * GET /task/{id} : Retrives the details of the task with id.
    * GET /resources-task/{task_id} : Retrives all the resources of the task with task_id.
    * POST /task : Creates a new task entity.
    * POST /resources-task  : Adds a resource to a task.
    * PUT /task  : Updates a task.
    * DELETE /task/{id} : Deletes the task with id.
* 0.1
    * Initial Release

