#ifndef INTERFACE_H
#define INTERFACE_H

enum interface_type {
    UNSET,
    CABLE,
    WIRELESS,
    UNNECESSARY
};

struct interface {
    const char* name;
    enum interface_type type;
    const char* label;
    struct interface* next;
};

struct interface* interface_new();
void interface_free(struct interface* this);

int interface_has_name(struct interface* this);
int interface_has_type(struct interface* this);
int interface_has_label(struct interface* this);

void interface_set_name(struct interface* this, const char* value);
void interface_set_type(struct interface* this, const char* value);
void interface_set_label(struct interface* this, const char* value);

void interface_infer(struct interface* this);
int interface_match(struct interface* this, const char* name);
void interface_validate(struct interface* this);

int is_cable_name(const char* name);
int is_wireless_name(const char* name);

#endif
