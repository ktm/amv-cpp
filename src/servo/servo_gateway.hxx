//
// Created by ktm on 5/13/16.
//

#ifndef AMVMODEL_SERVO_FACADE_HXX
#define AMVMODEL_SERVO_FACADE_HXX

#include <chrono>
#include <thread>
#include "../amv_configuration.hxx"

class servo_gateway {
    int serial_id;
public:
    void turn() {
        unsigned char deviceNumber = 12;
        unsigned char channelNumber = 0;

        // Generate a sinusoid signal to send to the PololuInterface
        const float pi = 3.141592653589793f;
        const unsigned int channelMinValue = 4000;
        const unsigned int channelMaxValue = 8000;
        const unsigned int channelValueRange = channelMaxValue - channelMinValue;
        const unsigned int signalPeriodInMs = 2000;
        clock_t t;
        int f;
        t = clock();
        int timeSinceStart = t/CLOCKS_PER_SEC;

        for (int channelValue = channelMinValue; channelValue < channelMaxValue; channelValue+=50) {
            printf("\rchannelValue=%d", (unsigned int)channelValue );
            unsigned char command[4] = { 0x84, channelNumber, (unsigned char)(channelValue & 0x7F), (unsigned char)((channelValue >> 7) & 0x7F) };

            amv_config->serial()->serial_write_bytes(serial_id, command, 4);

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
};
#endif //AMVMODEL_SERVO_FACADE_HXX
