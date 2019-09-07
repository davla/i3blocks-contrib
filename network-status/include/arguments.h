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

void parse_arguments(int argc, char** argv, struct args* args);
void if_args_free(struct if_args* this);
