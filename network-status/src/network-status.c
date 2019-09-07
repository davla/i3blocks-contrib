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

#include "arguments.h"
#include "common.h"
#include "interface.h"

int main(int argc, char** argv) {
    struct args args;

    parse_arguments(argc, argv, &args);
    interfaces_filter(args.interfaces);

    interface_free(args.interfaces);

    return 0;
}
