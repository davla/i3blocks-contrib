#include <getopt.h>

#include "arguments.h"
#include "common.h"

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

void if_args_validate(struct if_args* this) {
    while (this) {
        if (!this->name) {
            fprintf(stderr, "Missing interface name (type: %s, label: %s)\n",
                this->type, this->label);
            exit(EXIT_FAILURE);
        }
        if (!this->type) {
            fprintf(stderr, "Missing interface type (name: %s, label: %s)\n",
                this->name, this->label);
            exit(EXIT_FAILURE);
        }
        if (!this->label) {
            fprintf(stderr, "Missing interface label (name: %s, type: %s)\n",
                this->name, this->type);
            exit(EXIT_FAILURE);
        }
        this = this->next;
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

    if_args_validate(args->interfaces);
}
