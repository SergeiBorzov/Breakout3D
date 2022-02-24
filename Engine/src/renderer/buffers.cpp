#include <assert.h>

#include "../core/fly_gl.h"
#include "buffers.h"

namespace Fly {
    void VertexBuffer::Bind(const VertexBuffer& vb) {
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, vb.m_Handle));
    }

    void VertexBuffer::Unbind() {
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }
    
    void VertexBuffer::Create(const void* data, u64 size, BufferUsage bufferUsage) {
        assert(m_Handle == 0);
        GLCALL(glGenBuffers(1, &m_Handle));
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_Handle));

        GLenum usage;
        switch (bufferUsage) {
            case BufferUsage::StaticDraw: {
                usage = GL_STATIC_DRAW;
                break;
            }
            case BufferUsage::DynamicDraw: {
                usage = GL_DYNAMIC_DRAW;
                break;
            }
        }
        GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, usage));
    }

    void VertexBuffer::Destroy() {
        if (m_Handle)
            GLCALL(glDeleteBuffers(1, &m_Handle));
    }


    void IndexBuffer::Bind(const IndexBuffer& ib) {
        GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib.m_Handle));
    }

    void IndexBuffer::Unbind() {
        GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

    void IndexBuffer::Create(const void* data, u64 size, BufferUsage bufferUsage) {
        assert(m_Handle == 0);
        GLCALL(glGenBuffers(1, &m_Handle));
        GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle));
        
        GLenum usage;
        switch(bufferUsage) {
            case BufferUsage::StaticDraw: {
                usage = GL_STATIC_DRAW;
                break;
            }
            case BufferUsage::DynamicDraw: {                
                usage = GL_DYNAMIC_DRAW;
                break;
            }
        }
        GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage));
    }

    void IndexBuffer::Destroy() {
        if (m_Handle)
            GLCALL(glDeleteBuffers(1, &m_Handle));
    }

    void UniformBuffer::Bind(const UniformBuffer& ub) {
        GLCALL(glBindBuffer(GL_UNIFORM_BUFFER, ub.m_Handle));
    }

    void UniformBuffer::Unbind() {
        GLCALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
    }

    void UniformBuffer::BindBufferBase(const UniformBuffer& ub, u32 slot) {
        GLCALL(glBindBufferBase(GL_UNIFORM_BUFFER, slot, ub.m_Handle));
    }

    void UniformBuffer::ChangeData(const void* data, u64 size, u64 offset) {
        GLCALL(glBindBuffer(GL_UNIFORM_BUFFER, m_Handle));
        GLCALL(glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data));
    }

    void UniformBuffer::Create(const void* data, u64 size, BufferUsage bufferUsage) {
        assert(m_Handle == 0);
        GLCALL(glGenBuffers(1, &m_Handle));
        GLCALL(glBindBuffer(GL_UNIFORM_BUFFER, m_Handle));

        GLenum usage;
        switch(bufferUsage) {
            case BufferUsage::StaticDraw: {
                usage = GL_STATIC_DRAW;
                break;
            }
            case BufferUsage::DynamicDraw: {
                usage = GL_DYNAMIC_DRAW;
                break;
            }
        }
        GLCALL(glBufferData(GL_UNIFORM_BUFFER, size, data, usage));
    }

    void UniformBuffer::Destroy() {
        if (m_Handle)
            GLCALL(glDeleteBuffers(1, &m_Handle));
    }
    
}
