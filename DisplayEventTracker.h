/**
 * Author RogerYu, Written in 2022/04/16
 */

#ifndef DISPLAY_EVENT_TRACKER_H
#define DISPLAY_EVENT_TRACKER_H

#include <gui/DisplayEventReceiver.h>
#include <utils/Looper.h>
#include <utils/Thread.h>

namespace android {

class DisplayEventTracker : public Thread
{
public:
    explicit DisplayEventTracker(int vsyncRate);
    virtual ~DisplayEventTracker();

    void requestNextVsync();

private:
    virtual bool        threadLoop();
    virtual status_t    readyToRun();
    void processDisplayEvents();

    // Display event handling
    class DisplayEventCallback;

    std::unique_ptr<DisplayEventReceiver> mDisplayEventReceiver;
    sp<Looper> mLooper;
    int mVsyncRate;
};

// ---------------------------------------------------------------------------

}; // namespace android

#endif // DISPLAY_EVENT_TRACKER_H






