#include <assert.h>
#include "../core/fly_gl.h"
#include "vertex_array.h"

namespace Fly {
    static GLenum get_gl_type(Attribute::Type type) {
        GLenum result = 0;
        switch (type) {
            case Attribute::Type::Vec4:
            case Attribute::Type::Vec3:
            case Attribute::Type::Vec2:
            case Attribute::Type::f32: {
                result = GL_FLOAT;
                break;
            }
            case Attribute::Type::u32: {
                result = GL_UNSIGNED_INT;
                break;
            }
            case Attribute::Type::Color: {
                result = GL_UNSIGNED_BYTE;
            }
        }
        return result;
    }
    
    static u32 get_count(Attribute::Type type) {
        u32 result = 0;
        switch (type) {
            case Attribute::Type::u32:
            case Attribute::Type::f32: {
                result = 1;
                break;
            }
            case Attribute::Type::Vec2: {
                result = 2;
                break;
            }
            case Attribute::Type::Vec3: {
                result = 3;
                break;
            }
            case Attribute::Type::Color: 
            case Attribute::Type::Vec4: {
                result = 4;
                break;
            }
        }
        return result;
    }

    static u32 get_elem_size(Attribute::Type type) {
        u32 result = 0;
        switch (type) {
            case Attribute::Type::Vec4:
            case Attribute::Type::Vec3:
            case Attribute::Type::Vec2:
            case Attribute::Type::f32: {
                result = sizeof(f32);
                break;
            }
            case Attribute::Type::u32: {
                result = sizeof(u32);
                break;
            }
            case Attribute::Type::Color: {
                result = sizeof(u8);
                break;
            }
        }
        return result;
    }

    bool VertexArray::IsIndexFree(i32 index) {
        return (m_IndexMask & (u16)1 << index) == 0;
    }
    
    i32 VertexArray::NextFreeIndex() {
        for (int i = 0; i < 16; i++)
            if ((m_IndexMask & (u16)1 << i) == 0)
                return (i32)i;
        return -1;
    }
    
    void VertexArray::Bind(const VertexArray& va) {
        GLCALL(glBindVertexArray(va.m_Handle));
    }

    void VertexArray::Unbind() {
        GLCALL(glBindVertexArray(0));
    }

    void VertexArray::SetAttributes(const VertexBuffer& vb,
                                    const std::initializer_list<Attribute>& attributes) {
        if (m_Handle == 0)
            GLCALL(glGenVertexArrays(1, &m_Handle));

        VertexBuffer::Bind(vb);
        GLCALL(glBindVertexArray(m_Handle));

        u32 stride = 0;
        for (const Attribute& a: attributes) {
            stride += get_count(a.type)*get_elem_size(a.type);
        }

        u64 offset = 0;
        for (const Attribute& a: attributes) {
            i32 index;
            if (a.index >= 0) {
                index = a.index;
                assert(IsIndexFree(index));
            }
            else {
                index = NextFreeIndex();
                assert(index >= 0);
            }
             
            u32 count = get_count(a.type);
            GLenum glType = get_gl_type(a.type);

            if (glType == GL_UNSIGNED_INT) {
                GLCALL(glVertexAttribIPointer((u32)index, count, glType, stride,
                                              (void*)(offset)));
            }
            else {
                GLCALL(glVertexAttribPointer((u32)index, count, glType, a.normalized, stride,
                                             (void*)(offset)));
            }
            GLCALL(glEnableVertexAttribArray((u32)index));

           

            offset += get_count(a.type)*get_elem_size(a.type);
            m_IndexMask |= (u16)1 << index;
        }
    }

    void VertexArray::Destroy() {
        if (m_Handle) {
            glDeleteVertexArrays(1, &m_Handle);
        }
    }
}
