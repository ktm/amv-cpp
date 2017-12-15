//
// Created by ktm on 8/22/15.
//

#ifndef AMVMODEL_PROCESS_H
#define AMVMODEL_PROCESS_H

#include "context.hxx"
#include "event.hxx"
#include "process_node.hxx"
#include "../collection/name_value_pair.hxx"

#include <algorithm>
#include <list>
#include <map>
#include <string>

#ifdef TEST
#include "../../test/ProcessTest.hxx"
#endif

/** \enum ProcessLifecycle
 *  \brief Lifecycles of a process
 *  \details These are the details
 *
 */
enum class ProcessLifecycle {
    instantiated, started, running, stopped
};
/** \class Process
 *  \brief The composition and execution of a single process
 *  \details TBD
 *
 */
class Process {

    //FIXME: Process listens to events by name and processes some manner of execute node
    //       when an event comes in.
#ifdef TEST
    friend class ProcessTest;
#endif
    std::string processName;

    std::map<std::string, ProcessNodePtr> processNodeMap;

    ProcessNodePtr currentNode;

    ProcessLifecycle lifecycleState;

protected:
    ProcessNodePtr findNode(ProcessNodeT elementT, string elementName) {
        auto list = find_if(processNodeMap.begin(), processNodeMap.end(),
                            [elementT, elementName](const pair<string, ProcessNodePtr>&r) {
                                return ((r.first.compare(elementName) == 0) ? elementT== r.second->getProcessNodeType() : false);
                            });
        if (list == processNodeMap.end()) {
            return nullptr;
        }
        return list->second;
    }

   ProcessNodePtr findNode(string elementId) {
        auto list = find_if(processNodeMap.begin(), processNodeMap.end(),
                            [elementId](const pair<string, ProcessNodePtr>&r) {
                                return ((r.first.compare(elementId) == 0));
                            });
        if (list == processNodeMap.end()) {
            return nullptr;
        }

        return list->second;
    }

    void executeNode(ProcessNodePtr node) {
        currentNode = findNode(node->id);
        if (nullptr == currentNode) {
cout << "BLOW CHUNKS" << endl;
            return;
        }
        switch (node->getProcessNodeType()) {
            case ProcessNodeT::startEvent: {
                onStartEvent(StartEventPtr(std::dynamic_pointer_cast<StartEvent>(currentNode)));
                break;
            }
            case ProcessNodeT::signalEvent: {
                SignalEvent* se = (SignalEvent *) currentNode.get();
                for (const NameValuePairPtr nvp : SignalPtr(std::dynamic_pointer_cast<SignalEvent>(currentNode))->nvpList) {
                    Context::Instance().write(nvp->first, nvp->second);
                }
                break;
            }
            case ProcessNodeT::endEvent: {
                onEndEvent(EndEventPtr(std::dynamic_pointer_cast<EndEvent>(currentNode)));
                break;
            }
            default:
                break;
        }

        ProcessNodePtr nextNode = evaluateNextNode(currentNode);
        if (nextNode != nullptr) {
            executeNode(nextNode);
        }
    }

    void catchSignal(SignalPtr event) {
        // if currentnode != event then ignore
        // otherwise, find next node & execute
        if (currentNode == nullptr) {
            return;
        }
        if (currentNode->matches(event->getId())) {
            ProcessNodePtr nextNode = nullptr;
            nextNode = evaluateNextNode(currentNode);
            if (nextNode != nullptr) {
                executeNode(nextNode);
            }
        }
    }

    ProcessNodePtr evaluateNextNode(ProcessNodePtr node) {
        ProcessNodePtr retval = nullptr;
        if (node != nullptr) {
            retval = findNode(node->getNextNodeId());
        }
        return retval;
    }

    void onStartEvent(StartEventPtr event) {
        for (const NameValuePairPtr nvp : event->nvpList) {
            Context::Instance().write(nvp->first, nvp->second);
        }
        lifecycleState = ProcessLifecycle::started;
    }

    void onEndEvent(EndEventPtr event) {
        lifecycleState = ProcessLifecycle::stopped;
    }

public:
    Process(const char* idarg): processName(idarg), lifecycleState(ProcessLifecycle::instantiated) {}

    void addProcessNode(ProcessNodePtr node) {
        processNodeMap.emplace(node->getId(), node);
        if (node->elementType == ProcessNodeT::signalEvent) {
            std::string signalName = node->getId();
            EventCallbackContainer::Instance().addCallback(node->getId(), [this, signalName](NameValuePairPtr nvp){
                this->catchSignal(make_shared<SignalEvent>(signalName));
            });
        }
    }

    void start(std::string startNodeName) {
        ProcessNodePtr node = findNode(startNodeName);
        if (node == nullptr) {
            //blow chunks
            return;
        }
        executeNode(node);
    }
};

#endif //AMVMODEL_PROCESS_H
