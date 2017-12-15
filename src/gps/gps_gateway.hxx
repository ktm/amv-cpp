//
// Created by ktm on 5/13/16.
//

#ifndef AMVMODEL_GPS_FACADE_H
#define AMVMODEL_GPS_FACADE_H

#include "gps.hxx"
#include "nmea.h"

class gps;
class gps_gateway {
    gps* m_gps;

public:
    void init_gps(std::string portName);
    void update_gps_location();
};
#endif //AMVMODEL_GPS_FACADE_H
