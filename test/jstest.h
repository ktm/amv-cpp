//
// Created by ktm on 12/26/15.
//

#ifndef AMVMODEL_JAVASCRIPT_H
#define AMVMODEL_JAVASCRIPT_H

#include "../src/serial/serial.hxx"
#include "../src/gps/gps.hxx"

std::string fxn() {
    return "Hello World!";
}

class JSTest {
    int gpsFD;

public:
    JSTest() { }

    void testAllTheThings() {
        cout << "JSTest testAllTheThings begin..." << endl;
//        testSerial();
        testGPS();

        cout << "JSTest testAllTheThings end..." << endl;
    }

    void testSerial() {
        cout << "testSerial begin..." << endl;

        Context::Instance().write("gpsPort", "/dev/ttyUSB0");

        std::vector<std::string> args = {"gpsPort"};
        gpsFD = chaiscript_gateway::Instance().call<int>("serialInit", args);
        assert(gpsFD > 0);

        args = {"gpsFD"};
        Context::Instance().write("gpsFD", gpsFD);
        gpsFD = chaiscript_gateway::Instance().call<int>("serialConfig", args);

        Context::Instance().write("line", "testLine");
        Context::Instance().write("lineLength", 8);
        args = {"gpsFD", "line", "lineLength"};
        int readbytes = chaiscript_gateway::Instance().call<int>("serialRead", args);
        std::cout << "readBytes: " << readbytes << " buf: " << Context::Instance().read("line") << std::endl;

        args = {"gpsFD"};
        chaiscript_gateway::Instance().call<int>("serialClose", args);

        std::cout << "testSerial end..." << endl;
    }

    void testGPS() {
        cout << "testGPS begin..." << endl;

        Context::Instance().write("gpsPort", "/dev/ttyUSB0");

        std::vector<std::string> args = {"gpsPort"};
        gpsFD = chaiscript_gateway::Instance().call<int>("serialInit", args);
        assert(gpsFD > 0);

        args = {"gpsFD"};
        Context::Instance().write("gpsFD", gpsFD);
        gpsFD = chaiscript_gateway::Instance().call<int>("serialConfig", args);

        sleep(5);
        chaiscript_gateway::Instance().call<int>("updateGPS", args);

        std::cout << "latitude: " << boost::get<double>(Context::Instance().read("gps.latitude")) << std::endl;
        std::cout << "longitude: " << boost::get<double>(Context::Instance().read("gps.longitude")) << std::endl;
        std::cout << "altitude: " << boost::get<double>(Context::Instance().read("gps.altitude")) << " meters" << std::endl;

        cout << "testGPS passed" << endl;
    }

};
#endif //AMVMODEL_JAVASCRIPT_H
