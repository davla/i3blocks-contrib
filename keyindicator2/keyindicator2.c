#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>

#define CAPS_MASK (0x1)
#define NUM_MASK (0x2)

#define DEFAULT_ACTIVE_COLOR ("#00FF00")
#define DEFAULT_INACTIVE_COLOR ("#666666")

#ifndef ACTIVE_COLOR
    #define ACTIVE_COLOR_VALUE (ifnull(getenv("ACTIVE_COLOR"), DEFAULT_ACTIVE_COLOR))
    #define ACTIVE_COLOR_OPT "a:"
    #define ACTIVE_COLOR_LONG_INDEX (0)
#else
    #define ACTIVE_COLOR_VALUE (ACTIVE_COLOR)
    #define ACTIVE_COLOR_OPT ""
    #define ACTIVE_COLOR_LONG_INDEX (-1)
#endif
#ifndef CAPS_LABEL
    #define CAPS_LABEL (getenv("CAPS_LABEL"))
    #define CAPS_LABEL_OPT "c:"
    #define CAPS_LABEL_LONG_INDEX (ACTIVE_COLOR_LONG_INDEX + 1)
#else
    #define CAPS_LABEL_VALUE (CAPS_LABEL)
    #define CAPS_LABEL_OPT ""
    #define CAPS_LABEL_LONG_INDEX (ACTIVE_COLOR_LONG_INDEX)
#endif
#ifndef INACTIVE_COLOR
    #define INACTIVE_COLOR_VALUE (ifnull(getenv("INACTIVE_COLOR"), DEFAULT_INACTIVE_COLOR))
    #define INACTIVE_COLOR_OPT "i:"
    #define INACTIVE_COLOR_LONG_INDEX (CAPS_LABEL_LONG_INDEX + 1)
#else
    #define INACTIVE_COLOR_VALUE (INACTIVE_COLOR)
    #define INACTIVE_COLOR_OPT ""
    #define INACTIVE_COLOR_LONG_INDEX (CAPS_LABEL_LONG_INDEX)
#endif
#ifndef NUM_LABEL
    #define NUM_LABEL_VALUE (getenv("NUM_LABEL"))
    #define NUM_LABEL_OPT "n:"
    #define NUM_LABEL_LONG_INDEX (INACTIVE_COLOR_LONG_INDEX + 1)
#else
    #define NUM_LABEL_VALUE (NUM_LABEL)
    #define NUM_LABEL_OPT ""
    #define NUM_LABEL_LONG_INDEX (INACTIVE_COLOR_LONG_INDEX)
#endif

#define GETOPT_STRING (ACTIVE_COLOR_OPT CAPS_LABEL_OPT INACTIVE_COLOR_OPT NUM_LABEL_OPT)
#define HAS_LONG_OPTIONS (NUM_LABEL_LONG_INDEX > -1)

struct args {
    char* caps_label;
    char* num_label;
    char* active_color;
    char* inactive_color;
};

char* ifnull(char* a, char* b) {
    return a ? a : b;
}

void parse_arguments(int argc, char** argv, struct args* args) {
    /* Long options definition */
    static struct option long_options[] = {
        #ifndef ACTIVE_COLOR
            {"active-color", required_argument, 0, 0},
        #endif
        #ifndef CAPS_LABEL
            {"caps-label", required_argument, 0, 0},
        #endif
        #ifndef INACTIVE_COLOR
            {"inactive-color", required_argument, 0, 0},
        #endif
        #ifndef NUM_LABEL
            {"num-label", required_argument, 0, 0}
        #endif
    };
    int long_opt_index, opt;

    /* This array maps long option indices to the corresponding args fields */
    #if HAS_LONG_OPTIONS
        char** long_opts_to_args[NUM_LABEL_LONG_INDEX + 1];
        #ifndef ACTIVE_COLOR
            long_opts_to_args[ACTIVE_COLOR_LONG_INDEX] = &(args->active_color);
        #endif
        #ifndef CAPS_LABEL
            long_opts_to_args[CAPS_LABEL_LONG_INDEX] = &(args->caps_label);
        #endif
        #ifndef INACTIVE_COLOR
            long_opts_to_args[INACTIVE_COLOR_LONG_INDEX] = &(args->inactive_color);
        #endif
        #ifndef NUM_LABEL
            long_opts_to_args[NUM_LABEL_LONG_INDEX] = &(args->num_label);
        #endif
    #endif

    /* Checking environment variables and setting defaults */
    args->active_color = ACTIVE_COLOR_VALUE;
    args->caps_label = CAPS_LABEL_VALUE;
    args->inactive_color = INACTIVE_COLOR_VALUE;
    args->num_label = NUM_LABEL_VALUE;

    /* Parsing CLI options */
    while ((opt = getopt_long(argc, argv, GETOPT_STRING, long_options,
            &long_opt_index)) != -1) {
        switch (opt) {
            #if HAS_LONG_OPTIONS
                case 0:
                    *(long_opts_to_args[long_opt_index]) = optarg;
                    break;
            #endif

            #ifndef ACTIVE_COLOR
            case 'a':
                args->active_color = optarg;
                break;
            #endif

            #ifndef CAPS_LABEL
            case 'c':
                args->caps_label = optarg;
                break;
            #endif

            #ifndef INACTIVE_COLOR
            case 'i':
                args->inactive_color = optarg;
                break;
            #endif

            #ifndef NUM_LABEL
            case 'n':
                args->num_label = optarg;
                break;
            #endif

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

    #ifdef CAPS_LABEL
        show_indicator(leds & CAPS_MASK, args.caps_label, &args);
    #else
        if (args.caps_label) {
            show_indicator(leds & CAPS_MASK, args.caps_label, &args);
        }
    #endif

    #ifdef NUM_LABEL
        show_indicator(leds & NUM_MASK, args.num_label, &args);
    #else
        if (args.num_label) {
            show_indicator(leds & NUM_MASK, args.num_label, &args);
        }
    #endif

    printf("\n");
}
