//
// Created by ktm on 8/14/15.
//

#ifndef AMVMODEL_SIMPLESTARTEVENTS_H
#define AMVMODEL_SIMPLESTARTEVENTS_H


#include <assert.h>

#include "../src/model/process.hxx"
#include "../src/model/process_node.hxx"
#include "../src/model/event.hxx"

class ProcessTest {

public:
    ProcessTest(){}

    void testAllTheThings() {
        testContext();
        testStartEvent();
        testCatchSignal();
        testExclusiveGateway();
        testExclusiveGateway2();
        testExclusiveGateway3();
        testServiceTask();
    }

    void testContext() {
        cout << "testContext begin..." << endl;
        Process* p = new Process("anonymousStartEvent");
        StartEvent* se = new StartEvent("start");
        se->addNVP("nvp1", "testVal");
        p->addProcessNode(StartEventPtr(se));

        StartEventPtr testEvent(new StartEvent("start"));
        p->executeNode(testEvent);

        assert(p->currentNode->getId().compare("start") == 0);

        Context& c1 = Context::Instance();
        Value lval = c1.read("nvp1");
        Value rval = Value("testVal");
        assert(value_compare(lval, rval));

        cout << "testContext passed" << endl;
    }

    void testStartEvent() {
        cout << "testStartEvent begin..." << endl;
        Process* p = new Process("anonymousStartEvent");
        EndEvent* endEvent = new EndEvent("end");
        StartEvent* se = new StartEvent("start");
        se->setOutgoingNodeId("end");

        p->addProcessNode(StartEventPtr(se));
        p->addProcessNode(EndEventPtr(endEvent));

        StartEventPtr testEvent(new StartEvent("start"));
        p->executeNode(testEvent);

        assert(p->currentNode->getId().compare("end") == 0);
        assert(p->lifecycleState==ProcessLifecycle::stopped);
        cout << "testStartEvent passed." << endl;
    }

    void testCatchSignal() {
        cout << "testCatchSignal started..." << endl;
        Process* p = new Process("anonymousStartSignal");
        EndEvent* endEvent = new EndEvent("end");
        SignalEvent* signalEvent = new SignalEvent("SIG1");
        StartEvent* se = new StartEvent("start");

        se->setOutgoingNodeId("SIG1");
        signalEvent->setOutgoingNodeId("end");

        p->addProcessNode(StartEventPtr(se));
        p->addProcessNode(EndEventPtr(endEvent));
        p->addProcessNode(SignalPtr(signalEvent));
        StartEventPtr testEvent(new StartEvent("start"));
        p->executeNode(testEvent);
        EventCallbackContainer::Instance().fireEvent("SIG1");
        this_thread::sleep_for(2s);

        assert(p->currentNode->getId().compare("end") == 0);
        assert(p->lifecycleState==ProcessLifecycle::stopped);
        cout << "testCatchSignal passed." << endl;
    }

    void testExclusiveGateway() {
        cout << "testExclusiveGateway begin..." << endl;
        Process* p = new Process("anonymousStartEvent");

        ExclusiveGateway* testGate = new ExclusiveGateway("testGate");
        testGate->addFlow("x < 5", "endA");
        testGate->addFlow("x == 5", "endB");
        testGate->addFlow("x > 5", "endC");

        EndEvent* endEventA = new EndEvent("endA");
        EndEvent* endEventB = new EndEvent("endB");
        EndEvent* endEventC = new EndEvent("endC");

        StartEvent* se = new StartEvent("start");
        se->addNVP("x", 7);
        se->setOutgoingNodeId("testGate");

        p->addProcessNode(StartEventPtr(se));
        p->addProcessNode(ExclusiveGatewayPtr(testGate));
        p->addProcessNode(EndEventPtr(endEventA));
        p->addProcessNode(EndEventPtr(endEventB));
        p->addProcessNode(EndEventPtr(endEventC));

        StartEventPtr testEvent(new StartEvent("start"));
        p->executeNode(testEvent);

        assert(p->currentNode->getId().compare("endC") == 0);
        assert(p->lifecycleState==ProcessLifecycle::stopped);
        cout << "testExclusiveGateway passed." << endl;
    }

    void testExclusiveGateway2() {
        cout << "testExclusiveGateway2 begin..." << endl;
        Process* p = new Process("anonymousStartEvent");

        ExclusiveGateway* testGate = new ExclusiveGateway("testGate");
        testGate->addFlow("x < 5", "endA");
        testGate->addFlow("x == 5", "endB");
        testGate->addFlow("x > 5", "endC");

        EndEvent* endEventA = new EndEvent("endA");
        EndEvent* endEventB = new EndEvent("endB");
        EndEvent* endEventC = new EndEvent("endC");

        StartEvent* se = new StartEvent("start");
        se->addNVP("x", 5);
        se->setOutgoingNodeId("testGate");

        p->addProcessNode(StartEventPtr(se));
        p->addProcessNode(ExclusiveGatewayPtr(testGate));
        p->addProcessNode(EndEventPtr(endEventA));
        p->addProcessNode(EndEventPtr(endEventB));
        p->addProcessNode(EndEventPtr(endEventC));

        StartEventPtr testEvent(new StartEvent("start"));
        p->executeNode(testEvent);

        assert(p->currentNode->getId().compare("endB") == 0);
        assert(p->lifecycleState==ProcessLifecycle::stopped);
        cout << "testExclusiveGateway2 passed." << endl;
    }

    void testExclusiveGateway3() {
        cout << "testExclusiveGateway3 begin..." << endl;
        Process* p = new Process("anonymousStartEvent");

        ExclusiveGateway* testGate = new ExclusiveGateway("testGate");
        testGate->addFlow("x < 5", "endA");
        testGate->addFlow("x == 5", "endB");
        testGate->addFlow("x > 5", "endC");

        EndEvent* endEventA = new EndEvent("endA");
        EndEvent* endEventB = new EndEvent("endB");
        EndEvent* endEventC = new EndEvent("endC");

        StartEvent* se = new StartEvent("start");
        se->addNVP("x", -1);
        se->setOutgoingNodeId("testGate");

        p->addProcessNode(StartEventPtr(se));
        p->addProcessNode(ExclusiveGatewayPtr(testGate));
        p->addProcessNode(EndEventPtr(endEventA));
        p->addProcessNode(EndEventPtr(endEventB));
        p->addProcessNode(EndEventPtr(endEventC));

        StartEventPtr testEvent(new StartEvent("start"));
        p->executeNode(testEvent);

        assert(p->currentNode->getId().compare("endA") == 0);
        assert(p->lifecycleState==ProcessLifecycle::stopped);
        cout << "testExclusiveGateway3 passed." << endl;
    }

    /*
     * testExclusiveGateway sync and async
     * setup listeners on gateway criteria
     * sync test goes through in one call to executeNode
     * async halts on exclusiveGateway and waits for context change
     */
    void testServiceTask() {
        cout << "testServiceTask begin..." << endl;

        cout << "testServiceTask passed." << endl;
    }
};

#endif //AMVMODEL_SIMPLESTARTEVENTS_H
