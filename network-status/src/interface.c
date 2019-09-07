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

    new->name = "";
    new->type = UNSET;
    new->label = "";

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
    return *this->name != '\0';
}
int interface_has_type(struct interface* this) {
    return this->type != UNSET;
}
int interface_has_label(struct interface* this) {
    return *this->label != '\0';
}

void interface_set_name(struct interface* this, const char* value) {
    this->name = value;
    interface_infer_type(this);
}
void interface_set_label(struct interface* this, const char* value) {
    this->label = value;
}

void interface_infer_type(struct interface* this) {
    if (!(strcmp("ethernet", this->name) && strcmp("cable", this->name))
        || is_cable_name(this->name)) {
        this->type = CABLE;
    }
    else if (!(strcmp("wireless", this->name) && strcmp("wifi", this->name))
        || is_wireless_name(this->name)) {
        this->type = WIRELESS;
    }
    else if (interface_has_name(this)) {
        this->type = UNNECESSARY;
    }
}
void interface_infer_label(struct interface* this) {
    if (!interface_has_label(this)) {
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
void interface_infer(struct interface* this) {
    interface_infer_type(this);
    interface_infer_label(this);
}

int interface_match(struct interface* this, const char* name) {
    return !strcmp(name, this->name)
        || (this->type == CABLE && is_cable_name(name))
        || (this->type == WIRELESS && is_wireless_name(name));
}

void interface_validate(struct interface* this) {
    if (!interface_has_label(this)) {
        fprintf(stderr, "Interface misses label (name: %s, type: %s)\n",
            this->name, interface_type_str(this->type));
        exit(EXIT_FAILURE);
    }

    if (!(interface_has_type(this) || interface_has_name(this))) {
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
