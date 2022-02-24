#ifndef FLY_VERTEX_ARRAY_H
#define FLY_VERTEX_ARRAY_H

#include <initializer_list>

#include "../core/types.h"
#include "buffers.h"

namespace Fly {
    
    struct Attribute {
        enum Type {
            u32,
            f32,
            Vec2,
            Vec3,
            Vec4,
            Color
        };
        Type type;
        i32 index = -1;
        bool normalized = false;
    };
    
    class VertexArray {
    public:
        static void Bind(const VertexArray& va);
        static void Unbind();

        void SetAttributes(const VertexBuffer& vb, const std::initializer_list<Attribute>& attributes);

        void Destroy();
    private:
        bool IsIndexFree(i32 index);
        i32 NextFreeIndex();

        u32 m_Handle = 0;
        u16 m_IndexMask = 0; // OpenGL guarantees at least 16 vertex attributes
    };
    
}

#endif /* End of FLY_VERTEX_ARRAY_H*/
