//
// Created by ktm on 5/13/16.
//

#ifndef AMVMODEL_TEST_SERIAL_FACADE_HXX
#define AMVMODEL_TEST_SERIAL_FACADE_HXX

class test_serial_gateway: public serial_gateway {

int portIds = 0;

public:
    int serial_init(std::string portName) {
        std::cout << " serial_init " << portName << " " << std::endl;

        return portIds++;
    }

    int serial_write(int serialId, std::string s) {
        std::cout << " serial id " << serialId << " " << s << std::endl;
        return s.size();
    }

    int serial_write_bytes(int serialId, unsigned char* bytes, int size) {
        std::cout << " serial id " << serialId << " " << bytes << std::endl;

        return size;
    }

    std::string read_line(int serialId) {
        return "TEST LINE";
    }

};

#endif //AMVMODEL_SERIAL_FACADE_HXX
