#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xfixes.h>
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
    int fixes_event, fixes_error;
    int xi_major = 2, xi_minor = 0;

    if (!(d = XOpenDisplay(NULL))) {
        fprintf(stderr, "xhidecursor: couldn't open display %s\n",
                XDisplayName(NULL));
        return 1;
    }
    if (!XFixesQueryExtension(d, &fixes_event, &fixes_error)) {
        fprintf(stderr, "xhidecursor: XFixes extension is unavailable\n");
        XCloseDisplay(d);
        return 1;
    }
    if (XIQueryVersion(d, &xi_major, &xi_minor) != Success) {
        fprintf(stderr, "xhidecursor: XInput2 extension is unavailable\n");
        XCloseDisplay(d);
        return 1;
    }

    r = XDefaultRootWindow(d);
    xi_select_events(XI_RawKeyPress);
    XEvent e;
    while (XNextEvent(d, &e) == 0) {
        if (e.type != GenericEvent ||
            !XGetEventData(d, &e.xcookie))
            continue;

        XGenericEventCookie *c = &e.xcookie;
        switch (c->evtype) {
            case XI_RawKeyPress: {
                XIRawEvent *raw = (XIRawEvent *)c->data;
                KeySym keysym = XkbKeycodeToKeysym(d, (KeyCode)raw->detail,
                                                   0, 0);

                if (IsModifierKey(keysym))
                    break;

                xi_select_events(XI_RawMotion);
                XFixesHideCursor(d, r);
                break;
            }
            case XI_RawMotion:
                xi_select_events(XI_RawKeyPress);
                XFixesShowCursor(d, r);
                break;
        }
        XFreeEventData(d, c);
        /* Synchronize selection changes and discard stale events. */
        XSync(d, True);
    }
    XCloseDisplay(d);
    return 0;
}
