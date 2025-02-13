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

* Tasks:
    * GET /tasks : Retrives all the tasks available.
    * GET /tasks/{node_id} : Retrives all the tasks of node with node_id.
    * GET /task/{id} : Retrives a task entity by id.
    * GET /resources-task/{task_id} : Retrives the resource entities of the task by task_id.
    * POST /task : Creates a new task entity.
    * POST /resources-task  : Adds a resource to a task.
    * PUT /task  : Updates a task entity.
    * DELETE /task/{id} : Deletes a task entity by id.
      
* Example of json for POST /task
```
{
"name":"task1",
"description":"task_description",
"request":"2025-04-08",
"deadline":"2025-04-20",
"duration":"5",
"priority":"12",
"tasktype":"INTERVAL_BASED",
"node_id":"2"
}
```
* Example of json for POST /resources-task
  
```
{
"task_id":"1",
"resource_id":"4",
}
```
* Resources:
   * GET /resources : Retrives all the resource entities.
   * GET /resources/{id} : Retrives a resource entity by id.
   * GET /resources/task/{task_id} : Retrives all the resources of the task with task_id.
   * POST /resource : Creates a new resource entity.
   * DELETE /resource/{id} : Deletes a resource entity by id.

* Example of json for POST /resource
  
```
{
"name":"resource_name",
"capacity":"1"
}
```

* Nodes
    * GET /nodes : Retrives all the node entities.
    * GET /node/{id} : Retrives a node entity by id.
    * GET /nodes/{parent_id} : Retrives all the child nodes of the parent node with parent_id.
    * POST /node : Creates a new node entity.
    * DELETE /node/{id} : Deletes a node entity by id.

* Example of json for POST /resource
  
```
{
"name":"node_name",
"capacity":"2",
"minpriority":"10",
"maxpriority":"20",
"parent_node_id":"1"
}
```
* Schedule:
   * GET /schedule-report-node/{node_id}/{startDate}/{endDate} : Retrives the planned tasks of node with node_id betweeen startDate and endDate.
   * GET /schedule-report-all-node/{node_id}/{startDate}/{endDate} : Retrives the planned tasks of all the nodes of the tree starting from node with node_id betweeen startDate and endDate.
   * GET /schedule-report-resource/{resource_id}/{startDate}/{endDate} : Retrives the usage schedule of resource with resource_id betweeen startDate and endDate.
   * POST /schedule/{node_id} : Starts the scheduling algorithm for tasks of the node with node_id.
   * POST /schedule-all/{node_id} : Starts the scheduling algorithm for tasks of all the nodes of the tree starting from node with node_id.
