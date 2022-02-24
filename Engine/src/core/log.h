#ifndef FLY_LOG_H
#define FLY_LOG_H

#include <stdio.h>

#define FLY_WARN(format, ...) fprintf(stderr, format "\n", __VA_ARGS__)
#define FLY_LOG(format, ...) printf(format "\n", __VA_ARGS__)

#endif /* End of FLY_LOG_H */
