#pragma once

#include "../../include/models/CResource.hpp"
#include "../../include/services/Sql.hpp"

class IResourceService
{
public:
    virtual void init() = 0;
    virtual void addResource(const CResource *r) = 0;
    virtual json getAllResources() = 0;
    virtual json getResourceById(string id) = 0;
    virtual json getResourcesByTaskId(string task_id) = 0;
    virtual void deleteResourceById(string id) = 0;
    virtual void updateResourceById(string id) = 0;
};

class ResourceService : public IResourceService
{
public:
    void init() override;
    void addResource(const CResource *r) override;
    json getAllResources() override;
    json getResourceById(string id) override;
    json getResourcesByTaskId(string task_id) override;
    void deleteResourceById(string id) override;
    // TODO
    void updateResourceById(string id) override;
};