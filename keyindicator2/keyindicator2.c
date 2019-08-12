#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>

#define CAPS_MASK (0x1)
#define NUM_MASK (0x2)

#define DEFAULT_ACTIVE_COLOR ("#00FF00")
#define DEFAULT_INACTIVE_COLOR ("#666666")

struct args {
    char* caps_label;
    char* num_label;
    char* active_color;
    char* inactive_color;
};

void parse_arguments(int argc, char** argv, struct args* args) {
    char opt;

    /* Setting defaults */
    args->caps_label = NULL;
    args->num_label = NULL;
    args->active_color = DEFAULT_ACTIVE_COLOR;
    args->inactive_color = DEFAULT_INACTIVE_COLOR;

    /* Parsing CLI options */
    while ((opt = getopt(argc, argv, "a:c:i:n:")) != -1) {
        switch (opt) {
            case 'a':
                args->active_color = optarg;
                break;

            case 'c':
                args->caps_label = optarg;
                break;

            case 'i':
                args->inactive_color = optarg;
                break;

            case 'n':
                args->num_label = optarg;
                break;

            default:
                fprintf(stderr, "Unrecognized option: %c\n", opt);
                exit(EXIT_FAILURE);
        }
    }
}

unsigned long get_leds() {
    Display* display = XOpenDisplay(NULL);
    XKeyboardState keyboard;
    XGetKeyboardControl(display, &keyboard);
    return keyboard.led_mask;
}

void show_indicator(unsigned long led, const char* label,
    const struct args* args) {
    printf("<span color='%s'>%s</span>",
        led ? args->active_color : args->inactive_color, label);
}

int main(int argc, char** argv) {
    unsigned long leds;
    struct args args;

    parse_arguments(argc, argv, &args);
    leds = get_leds();

    if (args.caps_label) {
        show_indicator(leds & CAPS_MASK, args.caps_label, &args);
    }

    if (args.num_label) {
        show_indicator(leds & NUM_MASK, args.num_label, &args);
    }

    printf("\n");
}
