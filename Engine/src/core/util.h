#ifndef FLY_UTIL_H
#define FLY_UTIL_H

#include <stddef.h>

namespace Fly {
    namespace Util {
        // Free memory after use with free()
        char* ReadFileToString(const char* filename, size_t* string_size = nullptr);
    }
}

#endif /* End of FLY_UTIL_H */
