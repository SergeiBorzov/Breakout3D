#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "util.h"

namespace Fly {
    namespace Util {
        char* ReadFileToString(const char* filename, size_t* string_size) {
            FILE* f;
            if (fopen_s(&f, filename, "rb") != 0) {
                return nullptr;
            }

            if (fseek(f, 0L, SEEK_END) != 0) {
                fclose(f);
                return nullptr;
            }

            i64 file_size = ftell(f);
            size_t size = file_size + 1;
            
            if (string_size) {
                *string_size = size;
            }
            
            if (file_size == -1) {
                fclose(f);
                return nullptr;
            }
            rewind(f);

           
            char* res = (char*)calloc(1, size);
            if (!res) {
                fclose(f);
                return nullptr;
            }

            if (fread(res, file_size, 1, f) != 1) {
                fclose(f);
                free(res);
                return nullptr;
            }
            
            fclose(f);
            return res;
        }
    }
}

