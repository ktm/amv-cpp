//
// Created by ktm on 8/22/15.
//

#ifndef AMVMODEL_EVENT_H
#define AMVMODEL_EVENT_H

#include "process_node.hxx"

class BaseEvent {
    std::vector<NameValuePairPtr> nvpList;
    std::string eventName;

    friend class Process;

public:
    BaseEvent(string name): eventName(name) {
    }

    void addNVP(std::string name, Value value) {
        nvpList.push_back(make_shared<NameValuePair>(name, value));
    }

    string getEventName() {
        return eventName;
    }
};


class ProcessEvent: public BaseEvent, public ProcessNode {
public:
    ProcessEvent(ProcessNodeT t, string name): ProcessNode(t, name), BaseEvent(name) {}
};

class StartEvent: public ProcessEvent {
public:
    StartEvent(string name) : ProcessEvent(ProcessNodeT::startEvent, name) {
        setId(name);
    }
};

class EndEvent: public ProcessEvent {
public:
    EndEvent(string name) : ProcessEvent(ProcessNodeT::endEvent, name) {
        setId(name);
    }
};

class SignalEvent : public ProcessEvent {
public:
    SignalEvent(string name) : ProcessEvent(ProcessNodeT::signalEvent, name) {}
};

using EndEventPtr = shared_ptr<EndEvent>;
using StartEventPtr = shared_ptr<StartEvent>;
using SignalPtr = shared_ptr<SignalEvent>;


#endif //AMVMODEL_EVENT_H
