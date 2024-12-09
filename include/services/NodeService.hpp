#pragma once

#include "../../include/models/CNode.h"
#include "../../include/services/Sql.h"

class INodeService
{
public:
    virtual void init() = 0;
    virtual void addNode(const CNode *n) = 0;
    virtual json getAllNodes() = 0;
    virtual json getAllNodesOfParent(string parent_node_id) = 0;
    virtual json getNodeById(string id) = 0;
    virtual void deleteNodeById(string id) = 0;
    virtual void updateNodeById(string id) = 0;
};

class NodeService : public INodeService
{
public:
    void init() override;
    void addNode(const CNode *n) override;
    json getAllNodes() override;
    json getAllNodesOfParent(string parent_node_id) override;
    json getNodeById(string id) override;
    void deleteNodeById(string id) override;
    void updateNodeById(string id) override;
};