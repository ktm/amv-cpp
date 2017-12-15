#ifndef _GPS_H_
#define _GPS_H_

#include <string>
#include "../amv_configuration.hxx"
#include "../serial/serial_gateway.hxx"
#include "nmea.h"


struct location {
    double latitude;
    double longitude;
    double speed;
    double altitude;
    double course;
};
typedef struct location loc_t;


class gps {

    int serial_id;

// -------------------------------------------------------------------------
// Internal functions
// -------------------------------------------------------------------------

// convert deg to decimal deg latitude, (N/S), longitude, (W/E)
    void gps_convert_deg_to_dec(double *, char, double *, char);
    double gps_deg_dec(double);
public:
    gps(std::string port_name);
    void update_gps_location();
    // Get the actual location
    uint8_t process_gps_location(std::string, uint8_t);
};

#endif
