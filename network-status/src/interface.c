#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "interface.h"

const char* interface_type_str(const enum interface_type this) {
    switch (this) {
        case UNSET:
            return "UNSET";

        case CABLE:
            return "CABLE";

        case WIRELESS:
            return "WIRELESS";

        case UNNECESSARY:
            return "UNNECESSARY";

        default:
            return "";
    }
}

struct interface* interface_new() {
    struct interface* new;

    if (!(new = malloc(sizeof(struct interface)))) {
        perror("Couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    memset(new, 0, sizeof(struct interface));
    new->type = UNSET;

    return new;
}

void interface_free(struct interface* this) {
    struct interface* tmp;
    for (tmp = this; this; tmp = this) {
        this = this->next;
        free(tmp);
    }
}

int interface_has_name(struct interface* this) {
    return this->name != NULL;
}
int interface_has_type(struct interface* this) {
    return this->type != UNSET;
}
int interface_has_label(struct interface* this) {
    return this->label != NULL;
}

void interface_set_name(struct interface* this, const char* value) {
    this->name = value;
}
void interface_set_type(struct interface* this, const char* value) {
    if (!(strcmp("ethernet", value) && strcmp("cable", value))) {
        this->type = CABLE;
    }
    else if (!(strcmp("wireless", value) && strcmp("wifi", value))) {
        this->type = WIRELESS;
    }
}
void interface_set_label(struct interface* this, const char* value) {
    this->label = value;
}

void interface_infer(struct interface* this) {
    if (this->type == UNSET) {
        if (is_cable_name(this->name)) {
            this->type = CABLE;
        }
        else if (is_wireless_name(this->name)) {
            this->type = WIRELESS;
        }
        else if (this->name) {
            this->type = UNNECESSARY;
        }
    }

    if (!this->label) {
        switch (this->type) {
            case CABLE:
                this->label = "C";
                break;

            case WIRELESS:
                this->label = "W";
                break;

            default:
                break;
        }
    }
}

int interface_match(struct interface* this, const char* name) {
    return !strcmp(name, this->name)
        || (this->type == CABLE && is_cable_name(name))
        || (this->type == WIRELESS && is_wireless_name(name));
}

void interface_validate(struct interface* this) {
    if (!this->label) {
        fprintf(stderr, "Interface misses label (name: %s, type: %s)\n",
            this->name, interface_type_str(this->type));
        exit(EXIT_FAILURE);
    }

    if (this->type == UNSET && !this->name) {
        fprintf(stderr, "Interface misses both type and name (label :%s)\n",
            this->label);
        exit(EXIT_FAILURE);
    }
}

int is_cable_name(const char* name) {
    return !(strncmp("en", name, 2) && strncmp("eth", name, 3));
}
int is_wireless_name(const char* name) {
    return !(strncmp("wl", name, 2) && strncmp("wlan", name, 4));
}

// struct if_name* if_nameindex_copy(const struct if_nameindex* this) {
//     struct if_name* ret;
//
//     if (!(ret = malloc(sizeof(struct if_name)))) {
//         perror("Couldn't allocate memory")
//         exit(EXIT_FAILURE);
//     }
//
//     if (!(ret->name = malloc(sizeof(char) * strlen(this->if_name)))) {
//         perror("Couldn't allocate memory")
//         exit(EXIT_FAILURE);
//     }
//
//     ret->next = NULL;
//     strcpy(ret->name, this->if_name);
//
//     return ret;
// }
