#ifndef FLY_ERROR_H
#define FLY_ERROR_H

#include <stdio.h>
#include <stdlib.h>

// Macro that kills the app and prints the reason
#define FLY_ERROR(format, ...) do {                                                       \
    fprintf(stderr, "[FLY_ERROR]:%s: %d\n" format "\n", __FILE__, __LINE__, __VA_ARGS__); \
    exit(EXIT_FAILURE);                                                                   \
} while(0)                                                                   

#endif /* End of FLY_ERROR_H */
