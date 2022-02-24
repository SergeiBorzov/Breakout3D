#ifndef FLY_TEXTURE_H
#define FLY_TEXTURE_H

#include "../core/types.h"
#include "../resource/resource.h"

namespace Fly {
    class Texture: public Resource {
        friend class Loader;
    public:
        enum class Wrap {
            Clamp,
            Repeat,
            Mirrored
        };

        enum class Filtering {
            Nearest,
            Linear,
            Trilinear
        };

        void GenerateMipmaps();
        
        void SetWrapMode(Wrap wrapMode);
        void SetFilteringMode(Filtering filtering);

        inline Wrap GetWrapMode() const { return m_WrapMode; }
        inline Filtering GetFilteringMode() const { return m_Filtering; }
        
        inline u32 GetWidth() const { return m_Width; }
        inline u32 GetHeight() const { return m_Height; }
        inline u32 GetHandle() const { return m_Handle; }

        static void Bind(const Texture& texture);
        static void Unbind();
        static void ActiveTexture(u32 slot);

        static Texture* CreateBlackTexture();
    private:
        u32 m_Handle = 0;
        u32 m_Width = 0;
        u32 m_Height = 0;

        Wrap m_WrapMode = Wrap::Repeat;
        Filtering m_Filtering = Filtering::Linear;
    };
}
#endif /* End of FLY_TEXTURE_H */
