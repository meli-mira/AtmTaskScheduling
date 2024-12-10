#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#include "../include/models/CScheduler.h"
#include "../include/server.hpp"
#include "../include/router.hpp"
#include "../include/controllers/TaskController.h"
#include "../include/controllers/ResourceController.hpp"
#include "../include/controllers/TimetableController.hpp"
#include "../include/controllers/NodeController.hpp"
#include "../include/controllers/SchedulerController.hpp"

int main(int argc, char *argv[])
{
	try
	{
		if (argc != 4)
		{
			std::cerr << "Usage: TaskScheduling <address> <port> <threads>\n"
					  << "Example:\n"
					  << "    TaskScheduling 0.0.0.0 8080 1\n";
			return EXIT_FAILURE;
		}

		auto router = std::make_shared<Router>();
		auto server = Server(6969, router);

		router->setPrefix("/v1");

		// routes for tasks
		auto taskService = std::make_shared<TaskService>();
		taskService->init();
		auto taskController = std::make_shared<TaskController>(taskService);

		router->addRoute(GET, "/tasks", [taskController](auto &ctx)
						 { taskController->getTasks(ctx); });
		router->addRoute(GET, "/tasks/{node_id}", [taskController](auto &ctx)
						 { taskController->getTasksByNodeId(ctx); });
		router->addRoute(GET, "/task/{id}", [taskController](auto &ctx)
						 { taskController->getTaskById(ctx); });
		router->addRoute(GET, "/resources-task/{task_id}", [taskController](auto &ctx)
						 { taskController->getResourcesOfTask(ctx); });
		router->addRoute(POST, "/task", [taskController](auto &ctx)
						 { taskController->createTask(ctx); });
		router->addRoute(POST, "/resources-task", [taskController](auto &ctx)
						 { taskController->addResourceToTask(ctx); });
		router->addRoute(DELETE, "/task/{id}", [taskController](auto &ctx)
						 { taskController->deleteTask(ctx); });

		// routes for resources
		auto resourceService = std::make_shared<ResourceService>();
		resourceService->init();
		auto resourceController = std::make_shared<ResourceController>(resourceService);

		router->addRoute(GET, "/resources", [resourceController](auto &ctx)
						 { resourceController->getResources(ctx); });
		router->addRoute(GET, "/resources/{id}", [resourceController](auto &ctx)
						 { resourceController->getResourceById(ctx); });
		router->addRoute(GET, "/resources/task/{task_id}", [resourceController](auto &ctx)
						 { resourceController->getResourcesByTaskId(ctx); });
		router->addRoute(POST, "/resource", [resourceController](auto &ctx)
						 { resourceController->createResource(ctx); });
		router->addRoute(DELETE, "/resource/{id}", [resourceController](auto &ctx)
						 { resourceController->deleteResource(ctx); });

		// routes for timetable
		auto timetableService = std::make_shared<TimetableService>();
		timetableService->init();
		auto timetableController = std::make_shared<TimetableController>(timetableService);

		router->addRoute(GET, "/timetable/resource/{resource_id}", [timetableController](auto &ctx)
						 { timetableController->getTimetableByResourceId(ctx); });
		router->addRoute(GET, "/timetable/node/{node_id}", [timetableController](auto &ctx)
						 { timetableController->getTimetableByNodeId(ctx); });
		router->addRoute(GET, "/timetable/{id}", [timetableController](auto &ctx)
						 { timetableController->getTimetableById(ctx); });

		// routes for nodes
		auto nodeService = std::make_shared<NodeService>();
		nodeService->init();
		auto nodeController = std::make_shared<NodeController>(nodeService);

		router->addRoute(GET, "/nodes", [nodeController](auto &ctx)
						 { nodeController->getNodes(ctx); });
		router->addRoute(GET, "/nodes/{id}", [nodeController](auto &ctx)
						 { nodeController->getNodeById(ctx); });
		router->addRoute(GET, "/nodes/{parent_id}", [nodeController](auto &ctx)
						 { nodeController->getNodesByParentId(ctx); });
		router->addRoute(POST, "/node", [nodeController](auto &ctx)
						 { nodeController->createNode(ctx); });
		router->addRoute(DELETE, "/node/{id}", [nodeController](auto &ctx)
						 { nodeController->deleteNode(ctx); });

		// Routes for scheduling
		auto schedulerService = std::make_shared<SchedulerService>();
		auto schedulerController = std::make_shared<SchedulerController>(schedulerService);
		router->addRoute(POST, "/schedule/{node_id}", [schedulerController](auto &ctx)
						 { schedulerController->scheduleNode(ctx); });
		router->addRoute(POST, "/schedule-all/{node-id}", [schedulerController](auto &ctx)
						 { schedulerController->scheduleAllNodesFrom(ctx); });

		std::cout << "Starting server on port " << server.getPort() << endl;

		// start the scheduler instance
		CScheduler::getInstance()->init();

		// CScheduler::getInstance()->printAll();

		server.run();

		CScheduler::destroyInstance();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}

	return 0;
}
