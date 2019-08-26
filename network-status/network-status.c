// #if defined NO_CABLE && defined NO_WIRELESS
//     #error Both NO_CABLE and NO_WIRELESS defined
// #endif
//
// #if defined ACTIVE_COLOR && defined CABLE_LABEL && defined INACTIVE_COLOR && defined WIRELESS_LABEL
//     #define NO_ARGS
// #endif
//
// #ifndef ACTIVE_COLOR
//     #define ACTIVE_COLOR_HAS_OPT
// #endif
// #if ! (defined CABLE_LABEL || defined NO_CABLE)
//     #define CABLE_LABEL_HAS_OPT
// #endif
// #ifndef INACTIVE_COLOR
//     #define INACTIVE_COLOR_HAS_OPT
// #endif
// #if ! (defined WIRELESS_LABEL || defined NO_WIRELESS)
//     #define WIRELESS_LABEL_HAS_OPT
// #endif
//
// #ifdef NO_CABLE
//     #undef CABLE_LABEL
//     #define CABLE_LABEL (NULL)
// #endif
// #ifdef NO_WIRELESS
//     #undef WIRELESS_LABEL
//     #define WIRELESS_LABEL (NULL)
// #endif
//
// #define DEFAULT_ACTIVE_COLOR ("#00FF00")
// #define DEFAULT_INACTIVE_COLOR ("#666666")
//
// #ifdef ACTIVE_COLOR_HAS_OPT
//     #define ACTIVE_COLOR_VALUE (ifnull(getenv("ACTIVE_COLOR"), DEFAULT_ACTIVE_COLOR))
//     #define ACTIVE_COLOR_OPT "a:"
//     #define ACTIVE_COLOR_LONG_INDEX (0)
// #else
//     #define ACTIVE_COLOR_VALUE (ACTIVE_COLOR)
//     #define ACTIVE_COLOR_OPT ""
//     #define ACTIVE_COLOR_LONG_INDEX (-1)
// #endif
// #ifdef CABLE_LABEL_HAS_OPT
//     #define CABLE_LABEL_VALUE (getenv("CABLE_LABEL"))
//     #define CABLE_LABEL_OPT "c:"
//     #define CABLE_LABEL_LONG_INDEX (ACTIVE_COLOR_LONG_INDEX + 1)
// #else
//     #define CABLE_LABEL_VALUE (CABLE_LABEL)
//     #define CABLE_LABEL_OPT ""
//     #define CABLE_LABEL_LONG_INDEX (ACTIVE_COLOR_LONG_INDEX)
// #endif
// #ifdef INACTIVE_COLOR_HAS_OPT
//     #define INACTIVE_COLOR_VALUE (ifnull(getenv("INACTIVE_COLOR"), DEFAULT_INACTIVE_COLOR))
//     #define INACTIVE_COLOR_OPT "i:"
//     #define INACTIVE_COLOR_LONG_INDEX (CABLE_LABEL_LONG_INDEX + 1)
// #else
//     #define INACTIVE_COLOR_VALUE (INACTIVE_COLOR)
//     #define INACTIVE_COLOR_OPT ""
//     #define INACTIVE_COLOR_LONG_INDEX (CABLE_LABEL_LONG_INDEX)
// #endif
// #ifdef WIRELESS_LABEL_HAS_OPT
//     #define WIRELESS_LABEL_VALUE (getenv("WIRELESS_LABEL"))
//     #define WIRELESS_LABEL_OPT "n:"
//     #define WIRELESS_LABEL_LONG_INDEX (INACTIVE_COLOR_LONG_INDEX + 1)
// #else
//     #define WIRELESS_LABEL_VALUE (WIRELESS_LABEL)
//     #define WIRELESS_LABEL_OPT ""
//     #define WIRELESS_LABEL_LONG_INDEX (INACTIVE_COLOR_LONG_INDEX)
// #endif
//
// #define GETOPT_STRING (ACTIVE_COLOR_OPT CABLE_LABEL_OPT INACTIVE_COLOR_OPT WIRELESS_LABEL_OPT)
//
// #include <errno.h>
#ifndef NO_ARGS
    #include <getopt.h>
#endif
#include <ifaddrs.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct if_args {
    char* name;
    char* type;
    char* label;
    struct if_args* next;
};

struct args {
    struct if_args* interfaces;
    double good_level;
    double medium_level;
    char* good_color;
    char* medium_color;
    char* bad_color;
    char* down_color;
};

char** if_args_name(struct if_args* this) {
    return &(this->name);
}

char** if_args_type(struct if_args* this) {
    return &(this->type);
}

char** if_args_label(struct if_args* this) {
    return &(this->label);
}

struct if_args* if_args_new() {
    struct if_args* new;

    if (!(new = malloc(sizeof(struct if_args)))) {
        perror("Couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    memset(new, 0, sizeof(struct if_args));

    return new;
}

void if_args_add_field(struct if_args** ifs, char* value,
        char** (*get)(struct if_args*)) {
    struct if_args* this = *ifs;

    while (this && *get(this)) {
        ifs = &this->next;
        this = *ifs;
    }

    if (!this) {
        this = if_args_new();
        *ifs = this;
    }

    *get(this) = value;
}

void if_args_free(struct if_args* this) {
    for (struct if_args* tmp = this; this; tmp = this) {
        this = this->next;
        free(tmp);
    }
}

void handle_opt(int opt, struct args* args) {
    switch (opt) {
        case 0:
        case 'B':
            args->bad_color = optarg;
            break;

        case 1:
        case 'D':
            args->good_color = optarg;
            break;

        case 2:
        case 'M':
            args->medium_color = optarg;
            break;

        case 3:
        case 'g':
            args->good_level = atof(optarg);
            break;

        case 4:
        case 'h':
            // display help
            break;

        case 5:
        case 'i':
            if_args_add_field(&args->interfaces, optarg, if_args_name);
            break;

        case 6:
        case 'l':
            if_args_add_field(&args->interfaces, optarg, if_args_label);
            break;

        case 7:
        case 'm':
            args->medium_level = atof(optarg);
            break;

        case 8:
        case 't':
            if_args_add_field(&args->interfaces, optarg, if_args_type);
            break;

        default:
            /* getopt has already printed an error message */
            exit(EXIT_FAILURE);
    }
}

void parse_arguments(int argc, char** argv, struct args* args) {
    /* Long options definition */
    static struct option long_options[] = {
        {"bad-color", required_argument, 0, 0},
        {"down-color", required_argument, 0, 0},
        {"good-color", required_argument, 0, 0},
        {"medium-color", required_argument, 0, 0},
        {"good-level", required_argument, 0, 0},
        {"interface", required_argument, 0, 0},
        {"label", required_argument, 0, 0},
        {"medium-level", required_argument, 0, 0},
        {"type", required_argument, 0, 0},
    };
    int long_opt_index, opt;

    args->interfaces = NULL;
    args->good_level = 80.0;
    args->medium_level = 40.0;
    args->good_color = "#00FF00";
    args->medium_color = "#FFFF00";
    args->bad_color = "#FF0000";
    args->down_color = "#666666";

    while ((opt = getopt_long(argc, argv, "B:D:G:M:g:hi:l:m:t:", long_options,
            &long_opt_index)) != -1) {
        if (!opt) {
            handle_opt(long_opt_index, args);
        }
        else {
            handle_opt(opt, args);
        }
    }
}

int main(int argc, char** argv) {
    struct args args;

    parse_arguments(argc, argv, &args);
    for (struct if_args* curr = args.interfaces; curr; curr = curr->next) {
        printf("name: %s, type: %s, label: %s\n", curr->name, curr->type,
            curr->label);
    }
    // leds = get_leds();
    //
    // #ifndef NO_CAPS
    //     #ifdef CAPS_LABEL_HAS_OPT
    //         if (args.caps_label) {
    //             show_indicator(leds & CAPS_MASK, args.caps_label, &args);
    //         }
    //     #else
    //         show_indicator(leds & CAPS_MASK, args.caps_label, &args);
    //     #endif
    // #endif
    //
    // #ifndef NO_NUM
    //     #ifdef NUM_LABEL_HAS_OPT
    //         if (args.num_label) {
    //             show_indicator(leds & NUM_MASK, args.num_label, &args);
    //         }
    //     #else
    //         show_indicator(leds & NUM_MASK, args.num_label, &args);
    //     #endif
    // #endif

    printf("\n");

    if_args_free(args.interfaces);

    return 0;
}
