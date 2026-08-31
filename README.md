## Description

[xhidecursor](https://github.com/astier/xhidecursor) is a minimal X11 utility that hides the cursor on key presses and shows it again on mouse movement. Compared to more feature-rich alternatives such as [xbanish](https://github.com/jcs/xbanish), xhidecursor focuses on a simple implementation and efficient event processing, resulting in two main advantages:

- **Simplicity:** xhidecursor has only 66 SLOC compared to 488 SLOC for xbanish using, as measured by cloc. This is accomplished by using XFixes to hide and show the cursor and XInput2 to handle input events.

- **Performance:** When stress-tested on an i5-8350U by moving the mouse erratically, htop shows 0% CPU utilization for xhidecursor and up to 1.3% for xbanish. This is because xhidecursor only listens for the first mouse movement needed to show the cursor and ignores subsequent mouse movements, avoiding redundant event processing. The same applies to key presses.

## Dependencies

- libx11
- libxi
- libxfixes
- pkg-config

## Installation

```sh
make
sudo make install
```

## Usage
```sh
xhidecursor
```
