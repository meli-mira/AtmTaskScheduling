#pragma once

#include "../../include/models/CNotification.hpp"
#include "../../include/services/Sql.hpp"

class INotificationService
{
public:
    virtual void init() = 0;
    virtual void addNotification(const CNotification *n) = 0;
    virtual json getNotificationById(string id) = 0;
    virtual json getNotificationsByTaskId(string task_id) = 0;
    virtual json getNotificationsByNodeId(string node_id) = 0;
    virtual void deleteNotificationById(string id) = 0;
    virtual void updateNotification(CNotification *n) = 0;
};

class NotificationService : public INotificationService
{
public:
    void init() override;
    void addNotification(const CNotification *n) override;
    json getNotificationById(string id) override;
    json getNotificationsByTaskId(string task_id) override;
    json getNotificationsByNodeId(string node_id) override;
    void deleteNotificationById(string id) override;
    void updateNotification(CNotification *n) override;
};