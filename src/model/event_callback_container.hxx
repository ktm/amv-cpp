//
// Created by ktm on 9/25/15.
//

#ifndef AMVMODEL_EVENT_CONTAINER_HXX
#define AMVMODEL_EVENT_CONTAINER_HXX

#include <algorithm>
#include <map>
#include <mutex>
#include <iostream>
#include <thread>
#include <atomic>
#include "../collection/name_value_pair.hxx"
#include "../collection/thread_pool.hxx"

using EventCallback = function<void(NameValuePairPtr)>;
using EventCallbackMMap = std::multimap<std::string, EventCallback>;

class EventCallbackContainer {
private:

    thread_pool callback_thread_pool;
    std::mutex data_mutex;
    EventCallbackMMap callbackMap;

    EventCallbackContainer(){ }

    ~EventCallbackContainer(){}

public:
    static EventCallbackContainer& Instance() {
        static EventCallbackContainer BUS;
        return BUS;
    }

    void addCallback(std::string name, EventCallback cb) {
        std::lock_guard<std::mutex> data_lock(data_mutex);
        callbackMap.emplace(name, cb);
    }

    void fireEvent(NameValuePairPtr nvp) {
       fireEvent(nvp->first, nvp);
    }

    void fireEvent(std::string name) {
        fireEvent(name, nullptr);
    }

    void fireEvent(std::string name, NameValuePairPtr nvp) {
        std::pair <EventCallbackMMap::iterator, EventCallbackMMap::iterator> callbackRange;
        callbackRange = callbackMap.equal_range(name);
        for (auto item=callbackRange.first; item != callbackRange.second; ++item) {
            callback_thread_pool.submit(bind(item->second, nvp));
        }
    }
};


#endif
