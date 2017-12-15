//
// Created by ktm on 5/26/16.
//

#include "gps.hxx"
#include "gps_gateway.hxx"

void gps_gateway::init_gps(std::string portName) {
    m_gps = new gps(portName);
}

void gps_gateway::update_gps_location() {
    m_gps->update_gps_location();
}
