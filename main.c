#include <X11/extensions/XInput2.h>

#include <stdio.h>

static void xi_select_events(int);

static Display *d;
static Window r; // root-window

void xi_select_events(const int event) {
    unsigned char mask[3] = {None};
    XISetMask(mask, event);
    XIEventMask event_mask;
    event_mask.deviceid = XIAllMasterDevices;
    event_mask.mask_len = sizeof(mask);
    event_mask.mask = mask;
    XISelectEvents(d, r, &event_mask, 1);
}

int main(void) {
    if (!(d = XOpenDisplay(NULL))) {
        printf("Couldn't open Display.\n");
        return 1;
    }
    r = XDefaultRootWindow(d);
    xi_select_events(XI_RawKeyPress);
    XEvent e;
    XGenericEventCookie *c;
    while (!XNextEvent(d, &e)) {
        if (!XGetEventData(d, (c = &e.xcookie)))
            continue;
        switch (c->evtype) {
            case XI_RawKeyPress:
                xi_select_events(XI_RawMotion);
                XFixesHideCursor(d, r);
                break;
            case XI_RawMotion:
                xi_select_events(XI_RawKeyPress);
                XFixesShowCursor(d, r);
                break;
        }
        XFreeEventData(d, c);
    }
    XCloseDisplay(d);
}
