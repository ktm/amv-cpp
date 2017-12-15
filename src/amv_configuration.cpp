//
// Created by ktm on 6/25/16.
//
#include "amv_configuration.hxx"
#include "servo/servo_gateway.hxx"

std::shared_ptr<serial_gateway> amv_configuration::serial() {
    if (m_serial == nullptr) {
        m_serial = std::shared_ptr<serial_gateway>(new serial_gateway());
    }
    return m_serial;
}

std::shared_ptr<servo_gateway> amv_configuration::servo() {
    if (m_servo == nullptr) {
        m_servo = std::shared_ptr<servo_gateway>(new servo_gateway());
    }
    return m_servo;
}

std::shared_ptr<gps_gateway> amv_configuration::gps() {
    if (m_gps == nullptr) {
        m_gps = std::shared_ptr<gps_gateway>(new gps_gateway());
    }
    return m_gps;
}

void amv_configuration::initSerial() {
    chaiscript_gateway::Instance().js_context.add(chaiscript::fun(&serial_gateway::serial_init, serial().get()), "serialInit");
    chaiscript_gateway::Instance().js_context.add(chaiscript::fun(&serial_gateway::serial_write, serial().get()), "serialWrite");
    chaiscript_gateway::Instance().js_context.add(chaiscript::fun(&serial_gateway::read_line, serial().get()), "serialRead");
    chaiscript_gateway::Instance().js_context.add(chaiscript::fun(&serial_gateway::close, serial().get()),"serialClose");
}
