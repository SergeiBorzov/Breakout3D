#ifndef FLY_FONT_H
#define FLY_FONT_H

#include "../resource/resource.h"
#include "texture.h"

struct stbtt_fontinfo;

namespace Fly {
    class Font: public Resource {
        friend class Loader;
    public:
        inline f32 GetSize() const { return m_Size; }
        inline const Texture* GetBitmap() const { return m_Bitmap; }
        inline const void* GetBakedCharData() const { return m_BakedChars; }
        inline const stbtt_fontinfo* GetFontInfo() const { return m_FontInfo; }
        ~Font();
    private:
        u8* m_FontBuffer = nullptr;
        void* m_BakedChars = nullptr;
        stbtt_fontinfo* m_FontInfo = nullptr;
        Texture* m_Bitmap = nullptr;
        f32 m_Size = 0.0f;
    };
}

#endif /* End of FLY_FONT_H */