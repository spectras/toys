#ifndef MODULE_H
#define MODULE_H

#include <cstdint>
#include "config.h"

namespace customlink {

struct Entry
{
    using function_type = void();

    uint64_t        id;
    function_type * function;
};

#define CUSTOMLINK_MODULE(fnid) \
    static void fn_##fnid(); \
    static ::customlink::Entry __attribute__((section (".module_map"))) USED reg_##fnid = \
        { fnid, &fn_##fnid }; \
    static void fn_##fnid()

extern "C" const Entry moduleMap[];
extern "C" const Entry moduleMapEnd;

} // namespace customlink

#endif
