## Description

[xhidecursor](https://github.com/astier/xhidecursor) is a minimal X-application which hides the cursor on key-press and unhides the cursor on mouse-movement. The two main advantages compared to other popular alternatives like [xbanish](https://github.com/jcs/xbanish) are:

- **Simplicity:** xhidecursor `~40 SLOC` vs. xbanish `~488 SLOC`. This is because xhidecursor only uses the [XFIXES-Extension](https://cgit.freedesktop.org/xorg/proto/fixesproto/plain/fixesproto.txt) to hide the cursor while xbanish implements many different methods.

- **Performance:** If stress-tested on a i5-8350U CPU by moving the mouse erratically around htop shows a CPU-Utilization of `0%` for xhidecursor and up to `1.3%` for xbanish. This is because xhidecursor only listens to the first mouse-movement to unhide the cursor and ignores all the following mouse-movements. xbanish on the other hand processes every single mouse-movement even if the mouse is already visible. The same goes for key-presses.

## Dependencies

- libxi
- libxifixes

## Installation

```sh
make install
```
