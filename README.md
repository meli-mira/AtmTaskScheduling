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

Usage: TaskScheduling server_address port threads

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
    * PUT /unschedule-task/{task_id} : Unschedule the task with task_id, unscheduling the allocated resources and the associated node.
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
    * PUT /unschedule-node/{node_id}/{startDate}/{endDate} : Unschedule the node by node_id between startDate and endDate.
    * DELETE /node/{id} : Deletes a node entity by id.
    * 
* Example of json for POST /node
  
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
 
* Notification:
  * GET /notification/{notification_id} : Retrives a notification entity by id.
  * GET /notifications-node/{node_id} : Retrives all the notification entities of a node with node_id.
  * GET /notifications-task/{task_id} : Retrives all the notification entities of a task with task_id.
  * DELETE /notification/{id} : Deletes a notification entity by id.

## Usage example

1. Create the node "test_node".
```
{
"name":"test_node",
"capacity":"2",
"minpriority":"10",
"maxpriority":"20",
"parent_node_id":"1"
}
```
* The field capacity establish how many tasks can run in parallel in the specified node. In this example, the node would be able to run 2 tasks in pararel.
  
2. Create resources
   
* 4 resources will be added. For simplicity, each resource will have the capacity 1 (meaning it can be used only once in a time slot).
```
{
"name":"resource1",
"capacity":"1"
}
```
3. Add tasks to the created node.

* 4 task will be added. All 4 tasks have the request date: 08-04-2025 and the deadline: 20-04-2025. Moreover, for simplicity, the tasks will have the same priority and will be INTERVAL_BASED. 
* Task1:
     * Duration: 5 days
     * Resources used : resource1, resource2
* Task2:
     * Duration: 5 days
     * Resources used : resource1, resource3
* Task3:
     * Duration: 2 days
     * Resource used: resource3
* Task4:
     * Duration: 7 days
     * No resource used
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
![image](https://github.com/user-attachments/assets/36e70744-3f40-4611-98bb-a860d4161f10)

4. Allocate resources to tasks.
* In this case, the resources will have the id 10 - resource1, 11 - resource2, 12 - resource3, 13 -  resource4
```
{
"task_id":"13",
"resource_id":"10",
}
```
![image](https://github.com/user-attachments/assets/b8939729-994e-46db-beb4-3a2c758d961e)

6. Schedule the node tasks. POST /schedule/6 will produce the following result:
* Task1 will be planned between 08.04.2025 - 14.04.2025 and uses resource1 and resource2
* Task2 will not be planned as it uses resource1, already used by task1 between 08.04.2025 - 14.04.2025, and there is no enough space between 15.04.2025 - 20.04.2025 to plan it's 5 days. It will be generated a RESOURCE_ALREADY_IN_USE notification.
* Task3 will be planned between 08.04.2025 - 09.04.2025 (as the node has capacity 2) and it uses resource3.
* Task4 will be planned between 10.04.2025 - 18.04.2025.
![image](https://github.com/user-attachments/assets/ee51c21d-ffdd-4538-b167-7a60bd6f7cc7)

 
