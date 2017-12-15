//
// Created by ktm on 9/25/15.
//

#ifndef AMVMODEL_EVENTBUS_H
#define AMVMODEL_EVENTBUS_H

#include <iostream>

#include "../src/collection/name_value_pair.hxx"
#include "../src/model/event_callback_container.hxx"

class EventCallbackTest {

public:
    EventCallbackTest() { }

    void testAllTheThings() {
        testCallback();
    }

    void testCallback() {
        cout << "EventCallbackTest.testCallback begin..." << endl;
        static bool callbackFired = false;

        EventCallbackContainer::Instance().addCallback("testcb", [](NameValuePairPtr nvp){
            callbackFired = true;
        });

        NameValuePairPtr nvp = make_shared<NameValuePair> ("x", "xyz");
        EventCallbackContainer::Instance().fireEvent("testcb", nvp);
        this_thread::sleep_for(2s);
        assert(callbackFired);

        callbackFired = false;
        EventCallbackContainer::Instance().fireEvent("X", nvp);
        this_thread::sleep_for(2s);
        assert(!callbackFired);

        cout << "...EventCallbackTest.testCallback end" << endl;
    }

};
#endif //AMVMODEL_EVENTBUS_H
