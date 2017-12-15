//
// Created by ktm on 5/22/15.
//

#ifndef GPM_PROCESSMODEL_H
#define GPM_PROCESSMODEL_H

#include <algorithm>
#include <string>
#include <list>
#include <map>
#include <memory>
#include <vector>
#include <iostream>
#include "context.hxx"

using namespace std;

enum class ProcessNodeT {
    scriptTask, intermediateCatchEvent, exclusiveGateway, startEvent, endEvent, signalEvent
};

class ProcessNode {
    friend class Process;
    ProcessNodeT elementType;
    std::string outgoingSequenceId;

protected:
    std::string id = "NA";

public:
    ProcessNode(ProcessNodeT t) : elementType(t) {}

    ProcessNode(ProcessNodeT t, string idarg):id(idarg), elementType(t) {}

    void setOutgoingNodeId(std::string seq) {
        outgoingSequenceId = seq;
    }

    void setId(const char* idarg) {
        if (idarg != nullptr) {
            id = idarg;
        }
    }

    void setId(string idarg) {
        id = idarg;
    }

    std::string getId() {
        if (id.length() < 1) {
            id = "NA";
        }
        return id;
    };

    ProcessNodeT getProcessNodeType() {
        return elementType;
    }

    bool matches(string idarg) {
        return (getId().compare(idarg) == 0);
    }

    virtual std::string getNextNodeId() {
        return outgoingSequenceId;
    };
};

using ProcessNodePtr = shared_ptr<ProcessNode>;

class ScriptTask: public ProcessNode {
    std::string fileName;

public:
    ScriptTask(std::string idarg, std::string filenamearg) :
            ProcessNode(ProcessNodeT::scriptTask, idarg), fileName(filenamearg) {}
    void execute();
};


using GatewayFlow = pair<std::string, std::string>;
class ExclusiveGateway: public ProcessNode {
    std::vector<GatewayFlow> flows;

public:
    ExclusiveGateway(std::string name) : ProcessNode(ProcessNodeT::exclusiveGateway, name) {}

    void addFlow(std::string script, std::string nodeId) {
        flows.push_back(GatewayFlow(script, nodeId));
    }

    std::string getNextNodeId() {
        if (flows.size() < 1) {
            return ProcessNode::getNextNodeId();
        }
        for (auto iter = flows.begin() ; iter != flows.end(); ++iter) {
           if (chaiscript_gateway::Instance().evaluate_condition(iter->first)) {
               return iter->second;
           }
        }
        return nullptr;
    };
};

using ExclusiveGatewayPtr = shared_ptr<ExclusiveGateway>;


#endif //GPM_PROCESSMODEL_H
