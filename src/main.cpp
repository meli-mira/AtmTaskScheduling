#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#include "../include/models/CScheduler.hpp"
#include "../include/server.hpp"
#include "../include/router.hpp"
#include "../include/controllers/TaskController.hpp"
#include "../include/controllers/ResourceController.hpp"
#include "../include/controllers/TimetableController.hpp"
#include "../include/controllers/NodeController.hpp"
#include "../include/controllers/SchedulerController.hpp"
#include "../include/controllers/NotificationController.hpp"

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
		auto server = Server(atoi(argv[2]), router);

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
		router->addRoute(PUT, "/task", [taskController](auto &ctx)
						 { taskController->updateTask(ctx); });
		router->addRoute(PUT, "/unschedule-task/{task_id}", [taskController](auto &ctx)
						 { taskController->unscheduleTask(ctx); });
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
		router->addRoute(PUT, "/unschedule-node/{node_id}/{startDate}/{endDate}", [nodeController](auto &ctx)
						 { nodeController->unscheduleNode(ctx); });
		router->addRoute(DELETE, "/node/{id}", [nodeController](auto &ctx)
						 { nodeController->deleteNode(ctx); });

		// Routes for scheduling
		auto schedulerService = std::make_shared<SchedulerService>();
		auto schedulerController = std::make_shared<SchedulerController>(schedulerService);
		router->addRoute(POST, "/schedule/{node_id}", [schedulerController](auto &ctx)
						 { schedulerController->scheduleNode(ctx); });
		router->addRoute(POST, "/schedule-all/{node-id}", [schedulerController](auto &ctx)
						 { schedulerController->scheduleAllNodesFrom(ctx); });
		router->addRoute(GET, "/schedule-report-node/{node_id}/{startDate}/{endDate}", [schedulerController](auto &ctx)
						 { schedulerController->getSchedulingReportBetweenForNode(ctx); });
		router->addRoute(GET, "/schedule-report-all-node/{node_id}/{startDate}/{endDate}", [schedulerController](auto &ctx)
						 { schedulerController->getSchedulingReportBetweenFromNode(ctx); });
		router->addRoute(GET, "/schedule-report-resource/{resource_id}/{startDate}/{endDate}", [schedulerController](auto &ctx)
						 { schedulerController->getSchedulingReportBetweenForResource(ctx); });

		// Routes for notifications
		auto notificationService = std::make_shared<NotificationService>();
		notificationService->init();
		auto notificationController = std::make_shared<NotificationController>(notificationService);
		router->addRoute(GET, "/notification/{notification_id}", [notificationController](auto &ctx)
						 { notificationController->getNotificationById(ctx); });
		router->addRoute(GET, "/notifications-node/{node_id}", [notificationController](auto &ctx)
						 { notificationController->getNotificationsByNodeId(ctx); });
		router->addRoute(GET, "/notifications-task/{task_id}", [notificationController](auto &ctx)
						 { notificationController->getNotificationsByTaskId(ctx); });
		router->addRoute(DELETE, "/notification/{id}", [notificationController](auto &ctx)
						 { notificationController->deleteNotification(ctx); });

		std::cout << "Starting server on port " << server.getPort() << endl;

		// Start the scheduler instance
		CScheduler::getInstance()->init();

		server.run();

		CScheduler::destroyInstance();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}

	return 0;
}
