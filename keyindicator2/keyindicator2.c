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
    /* Long options definition */
    static struct option long_options[] = {
        {"active-color", required_argument, 0, 0},
        {"caps-label", required_argument, 0, 0},
        {"inactive-color", required_argument, 0, 0},
        {"num-label", required_argument, 0, 0}
    };
    int long_opt_index, opt;

    /* This array maps long option indices to the corresponding args fields */
    char** long_opts_to_args[4];
    long_opts_to_args[0] = &(args->active_color);
    long_opts_to_args[1] = &(args->caps_label);
    long_opts_to_args[2] = &(args->inactive_color);
    long_opts_to_args[3] = &(args->num_label);

    /* Setting defaults */
    args->caps_label = NULL;
    args->num_label = NULL;
    args->active_color = DEFAULT_ACTIVE_COLOR;
    args->inactive_color = DEFAULT_INACTIVE_COLOR;

    /* Parsing CLI options */
    while ((opt = getopt_long(argc, argv, "a:c:i:n:", long_options,
            &long_opt_index)) != -1) {
        switch (opt) {
            case 0:
                *(long_opts_to_args[long_opt_index]) = optarg;
                break;

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
                /* getopt has already printed an error message */
                exit(EXIT_FAILURE);
        }
    }
}

unsigned long get_leds() {
    XKeyboardState keyboard;
    Display* display = XOpenDisplay(NULL);
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
