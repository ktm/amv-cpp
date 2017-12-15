#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include "serial/serial_gateway.hxx"
#include "gps/gps_gateway.hxx"
#include "script/chaiscript_gateway.h"

class gps_gateway;
class serial_gateway;
class servo_gateway;

class amv_configuration {

protected:
    std::shared_ptr<gps_gateway> m_gps;
    std::shared_ptr<servo_gateway> m_servo;
    std::shared_ptr<serial_gateway> m_serial;
    virtual void initSerial();

public:

    std::shared_ptr<serial_gateway> serial();
    std::shared_ptr<servo_gateway>  servo();
    std::shared_ptr<gps_gateway> gps();
};

extern amv_configuration* amv_config;

#endif
