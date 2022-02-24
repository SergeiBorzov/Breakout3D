#include <stdlib.h>

#include "font.h"

namespace Fly {
    Font::~Font() {
        if (m_FontBuffer) {
            free(m_FontBuffer);
        }
        
        if (m_FontInfo) {
            free(m_FontInfo);
        }
        
        if (m_BakedChars) {
            free(m_BakedChars);
        }

        if (m_Bitmap) {
            delete m_Bitmap;
        }
    }
}