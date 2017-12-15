//
// Created by ktm on 5/13/16.
//

#ifndef AMVMODEL_SERIAL_FACADE_HXX
#define AMVMODEL_SERIAL_FACADE_HXX

#include <thread>
#include "serial.hxx"

class serial_gateway {
    using serial_stream_ptr = std::shared_ptr<serial_stream>;
    using serial_stream_ptr_vec = std::vector<serial_stream_ptr>;

    serial_stream_ptr_vec serial_stream_vector;

    int addStream(serial_stream_ptr p) {
        serial_stream_vector.push_back(p);
        return serial_stream_vector.size() - 1;
    }

    void removeStream(int idx) {
        if (serial_stream_vector.size() > idx) {
            serial_stream_vector.erase(serial_stream_vector.begin()+idx);
        }
    }

public:
    virtual int serial_init(std::string portName) {
        auto ssp = std::make_shared<serial_stream>(portName, 9600);

        return addStream(ssp);
    }

    virtual void close(int serialId) {
        serial_stream_vector[serialId]->close();

    }

    virtual int serial_write(int serialId, std::string s) {
        serial_stream_vector[serialId]->writeString(s);
        return s.size();
    }

    virtual int serial_write_bytes(int serialId, unsigned char* bytes, int size) {
        serial_stream_vector[serialId]->writeBytes(bytes, size);
        return size;
    }

    virtual std::string read_line(int serialId) {
        return serial_stream_vector[serialId]->readLine();
    }
};

#endif //AMVMODEL_SERIAL_FACADE_HXX
