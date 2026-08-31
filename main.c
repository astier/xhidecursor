#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/XInput2.h>

#include <stdio.h>

static void xi_select_events(int);

static Display *d;
static Window r;

static void xi_select_events(const int event) {
    unsigned char mask[XIMaskLen(XI_LASTEVENT)] = {0};
    XISetMask(mask, event);
    XIEventMask event_mask = {
        .deviceid = XIAllMasterDevices,
        .mask_len = sizeof(mask),
        .mask = mask
    };
    XISelectEvents(d, r, &event_mask, 1);
}

int main(void) {
    /* Check runtime requirements. */
    if (!(d = XOpenDisplay(NULL))) {
        fprintf(stderr, "xhidecursor: cannot open display %s\n", XDisplayName(NULL));
        return 1;
    }
    if (!XFixesQueryExtension(d, &(int){0}, &(int){0})) {
        fprintf(stderr, "xhidecursor: XFixes extension is required\n");
        XCloseDisplay(d);
        return 1;
    }
    if (XIQueryVersion(d, &(int){2}, &(int){0}) != Success) {
        fprintf(stderr, "xhidecursor: XInput2 version 2.0 or later is required\n");
        XCloseDisplay(d);
        return 1;
    }
    // Process input events.
    r = XDefaultRootWindow(d);
    xi_select_events(XI_RawKeyPress);
    XEvent e;
    while (!XNextEvent(d, &e)) {
        if (e.type != GenericEvent ||
            !XGetEventData(d, &e.xcookie))
            continue;
        XGenericEventCookie *c = &e.xcookie;
        switch (c->evtype) {
            // XSync discards queued events to prevent cursor-state races between key presses and mouse movement.
            case XI_RawKeyPress: {
                // Get the keysym to check whether the pressed key is a modifier.
                const XIRawEvent *raw_e = c->data;
                const KeyCode keycode = (KeyCode)raw_e->detail;
                const KeySym keysym = XkbKeycodeToKeysym(d, keycode, 0, 0);
                if (!IsModifierKey(keysym)) {
                    xi_select_events(XI_RawMotion);
                    XFixesHideCursor(d, r);
                    XSync(d, True);
                }
                break;
            } case XI_RawMotion:
                xi_select_events(XI_RawKeyPress);
                XFixesShowCursor(d, r);
                XSync(d, True);
                break;
            default:
                break;
        }
        XFreeEventData(d, c);
    }
    XCloseDisplay(d);
    return 0;
}
