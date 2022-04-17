/**
 * Author RogerYu, Written in 2022/04/16
 */

#include <utils/Errors.h>
#include <utils/Log.h>

#include <gui/DisplayEventReceiver.h>

#include "DisplayEventTracker.h"

namespace android {

class DisplayEventTracker::DisplayEventCallback : public LooperCallback {
    nsecs_t oldTimeStamp;
public:
    DisplayEventCallback(): oldTimeStamp(0) {}

    int handleEvent(int /* fd */, int /*events*/, void* data) {
        DisplayEventReceiver* displayEventReceiver = (DisplayEventReceiver*)data;
        constexpr int kBufferSize = 100;
        DisplayEventReceiver::Event buffer[kBufferSize];
        ssize_t numEvents;
        do {
            numEvents = displayEventReceiver->getEvents(buffer, kBufferSize);
            for (size_t i = 0; i < static_cast<size_t>(numEvents); i++) {
                const auto& event = buffer[i];
                if (event.header.type == DisplayEventReceiver::DISPLAY_EVENT_HOTPLUG) {
                    printf("Hotplug received: %s\n", event.hotplug.connected?"connected":"disconnected");
                }

                if(event.header.type == DisplayEventReceiver::DISPLAY_EVENT_VSYNC) {
                    printf("Vsync received: count=%d\t", event.vsync.count);
                    if (oldTimeStamp) {
                        float t = float(event.header.timestamp - oldTimeStamp) / s2ns(1);
                        printf("%f ms (%f Hz)\n", t*1000, 1.0/t);
                    } else {
                        printf("\n");
                    }
                    oldTimeStamp = event.header.timestamp;
                }

                if(event.header.type == DisplayEventReceiver::DISPLAY_EVENT_MODE_CHANGE) {
                    printf("Mode change received\n");
                }
                
            }
        } while (numEvents > 0);

        return 1;  // keep the callback
    }
};

DisplayEventTracker::DisplayEventTracker(int vsyncRate)
        : Thread(false), mLooper(new Looper(false)), mVsyncRate(vsyncRate) {
}

DisplayEventTracker::~DisplayEventTracker() {
}

status_t DisplayEventTracker::readyToRun() {
    // Register a display event receiver
    mDisplayEventReceiver = std::make_unique<DisplayEventReceiver>();
    status_t status = mDisplayEventReceiver->initCheck();
    if(status != NO_ERROR)
        printf("Initialization of DisplayEventReceiver failed with status: %d", status);

    mLooper->addFd(mDisplayEventReceiver->getFd(), 0, Looper::EVENT_INPUT,
            new DisplayEventCallback(), mDisplayEventReceiver.get());
    // setVsyncRate() sets the Event::VSync delivery rate. 
    // A value of 1 returns every Event::VSync.
    // A value of 2 returns every other event, etc...
    // a value of 0 returns no event unless  requestNextVsync() has been called.
    mDisplayEventReceiver->setVsyncRate(mVsyncRate);
    
    return NO_ERROR;
}

bool DisplayEventTracker::threadLoop() {
    processDisplayEvents();
    return true;
}

void DisplayEventTracker::processDisplayEvents() {
    // This will poll mDisplayEventReceiver and if there are new events it'll call
    // displayEventCallback synchronously.
    mLooper->pollOnce(-1);
}

void DisplayEventTracker::requestNextVsync() {
    mDisplayEventReceiver->requestNextVsync();
}

} // namespace android
