#ifndef FLY_FRAMEBUFFER_H
#define FLY_FRAMEBUFFER_H

#include "../core/types.h"

namespace Fly {
    class Framebuffer {
    public:
        enum BindMode {
            ReadWrite,
            Read,
            Write
        };

        static void Bind(const Framebuffer* fb, BindMode = ReadWrite);
        static void Unbind();

        u32 GetColorAttachmentHandle() const { return m_ColorHandle; }
        u32 GetDepthStencilAttachmentHandle() const { return m_DepthStencilHandle; }

        i32 Create(bool multisampled = false);
        void Resize(i32 width, i32 height);
        void Destroy();
    private:
        u32 m_Handle = 0;

        u32 m_ColorHandle = 0;
        u32 m_DepthStencilHandle = 0;
        bool m_Multisampled = false;
    };
}

#endif /* End of FLY_FRAMEBUFFER_H */