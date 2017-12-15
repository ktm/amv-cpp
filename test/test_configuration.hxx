//
// Created by ktm on 5/18/16.
//

#ifndef AMVMODEL_TEST_CONFIGURATION_BUILDER_HXX
#define AMVMODEL_TEST_CONFIGURATION_BUILDER_HXX


#include "../src/amv_configuration.hxx"
#include "../src/gps/gps_gateway.hxx"
#include "test_serial_gateway.hxx"

void writeContextValue(std::string name, chaiscript::Boxed_Value value) {
    chaiscript_gateway::Instance().write_from_js(name, value);
}

void scriptPause(chaiscript::Boxed_Value v) {
    std::chrono::time_point<std::chrono::system_clock> start, end;

    start = std::chrono::system_clock::now();
    std::time_t start_time = std::chrono::system_clock::to_time_t(start);
    std::cout << std::ctime(&start_time) << std::endl;

    int seconds = chaiscript::boxed_cast<int>(v);
    std::this_thread::sleep_for (std::chrono::seconds(seconds));

    end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << std::ctime(&end_time) << std::endl;
}

class test_configuration: public amv_configuration {

public:

    test_configuration() {
        m_serial = std::shared_ptr<serial_gateway>(new test_serial_gateway());
        chaiscript_gateway::Instance().js_context.add(chaiscript::fun(&serial_gateway::serial_init, serial().get()), "serialInit");
        chaiscript_gateway::Instance().js_context.add(chaiscript::fun(&serial_gateway::serial_write, serial().get()), "serialWrite");
        chaiscript_gateway::Instance().js_context.add(chaiscript::fun(&serial_gateway::read_line, serial().get()), "serialRead");
        chaiscript_gateway::Instance().js_context.add(chaiscript::fun(&serial_gateway::close, serial().get()),"serialClose");
        chaiscript_gateway::Instance().registerFunction(writeContextValue, "writeContext");
        chaiscript_gateway::Instance().registerFunction(scriptPause, "sleep");
    }
};

#endif //AMVMODEL_TEST_CONFIGURATION_BUILDER_HXX