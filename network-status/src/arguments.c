#include <getopt.h>

#include "arguments.h"
#include "common.h"
#include "interface.h"

void interfaces_add_field(struct interface** ifs, char* value,
        int (*has_field)(struct interface*),
        void (*set_field)(struct interface*, const char*)) {
    struct interface* this = *ifs;

    while (this && has_field(this)) {
        ifs = &this->next;
        this = *ifs;
    }

    if (!this) {
        this = interface_new();
        *ifs = this;
    }

    set_field(this, value);
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
            interfaces_add_field(&args->interfaces, optarg,
                interface_has_name, interface_set_name);
            break;

        case 6:
        case 'l':
            interfaces_add_field(&args->interfaces, optarg,
                interface_has_label, interface_set_label);
            break;

        case 7:
        case 'm':
            args->medium_level = atof(optarg);
            break;

        case 8:
        case 't':
            interfaces_add_field(&args->interfaces, optarg,
                interface_has_type, interface_set_type);
            break;

        default:
            /* getopt has already printed an error message */
            exit(EXIT_FAILURE);
    }
}

void parse_arguments(int argc, char** argv, struct args* args) {
    struct interface* ifs;

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

    for (ifs = args->interfaces; ifs; ifs = ifs->next) {
        interface_infer(ifs);
        interface_validate(ifs);
    }
}
