#ifndef FLY_BUFFERS_H
#define FLY_BUFFERS_H

#include "../core/types.h"

namespace Fly {

    enum class BufferUsage {
        StaticDraw,
        DynamicDraw
    };
    
    class VertexBuffer {
    public:
        void Create(const void* data, u64 size, BufferUsage bufferUsage);
        static void Bind(const VertexBuffer& vb);
        static void Unbind();
        void Destroy();
    private:
        u32 m_Handle = 0;        
    };

    class IndexBuffer {
    public:
        void Create(const void* data, u64 size, BufferUsage bufferUsage);
        static void Bind(const IndexBuffer& ib);
        static void Unbind();
        void Destroy();
    private:
        u32 m_Handle = 0;
    };

    class UniformBuffer {
    public:
        static void Bind(const UniformBuffer& ub);
        static void Unbind();
        static void BindBufferBase(const UniformBuffer& ub, u32 slot);
        
        void Create(const void* data, u64 size, BufferUsage bufferUsage);
        void ChangeData(const void* data, u64 size, u64 offset);
        void Destroy();
    private:
        u32 m_Handle = 0;
    };
}

#endif /* End of FLY_BUFFERS_H */
