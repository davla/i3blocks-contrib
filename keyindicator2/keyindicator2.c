#include <stdio.h>
#include <X11/Xlib.h>

#define CAPS_MASK (0x1)
#define NUM_MASK (0x2)

int main(int argc, char** argv) {
    Display* display = XOpenDisplay("");
    XKeyboardState keyboard;

    XGetKeyboardControl(display, &keyboard);

    char* active_color = "#00FF00";
    char* inactive_color = "#FFFFFF";

    int is_caps_on = keyboard.led_mask & CAPS_MASK;
    int is_num_on = keyboard.led_mask & NUM_MASK;

    char* caps_color = is_caps_on ? active_color : inactive_color;
    char* num_color = is_num_on ? active_color : inactive_color;

    printf("<span color=\"%s\">C</span><span color=\"%s\">N</span>\n",
        caps_color, num_color);
}
