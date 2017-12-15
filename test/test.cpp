//
// Created by ktm on 8/14/15.
//

#define TEST

#include <ev.h>
#include "ProcessTest.hxx"
#include "jstest.h"
#include "../src/amv_configuration.hxx"
#include "test_configuration.hxx"

chaiscript::Boxed_Value scriptObject;
std::function<std::string(chaiscript::Boxed_Value &, const std::string &)> funcObject;
ev_timer evTimer;

static void timer_cb (EV_P_ ev_timer *w, int revents)  {
    std::cout << "c++ timer_cb " << std::endl;
    auto result = funcObject(scriptObject, "timer bits");
}

amv_configuration* amv_config;

int mainTest(std::string file_name) {
    amv_config = new test_configuration();

    auto obj = chaiscript_gateway::Instance().evaluateFile(file_name);
/*

    JSTest jsTest;
    jsTest.testAllTheThings();
    ContextTest contextTest;
    contextTest.testAllTheThings();

    EventCallbackTest eventBusTest;
    eventBusTest.testAllTheThings();

    ProcessTest sse;
    sse.testAllTheThings();
    */

/*
    scriptObject = chaiscript_gateway::Instance().js_context.eval("amv");
    funcObject = chaiscript_gateway::Instance().js_context.eval<std::function<std::string(chaiscript::Boxed_Value &, const std::string &)>>("onTimer");

    struct ev_loop *loop = EV_DEFAULT;
    ev_timer_init (&evTimer, timer_cb, 1., 5.);
    ev_timer_start (loop, &evTimer);
    ev_run (loop, 0);
*/
    return 0;
}
