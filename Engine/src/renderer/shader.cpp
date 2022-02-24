#include "../core/fly_gl.h"
#include "shader.h"

namespace Fly {
    void Shader::Use(const Shader& shader) {
        GLCALL(glUseProgram(shader.m_Handle));
    }

    void Shader::SetUniform(const char* name, i32 value) const {
        u32 index;
        GLCALL(index = glGetUniformLocation(m_Handle, name));
        GLCALL(glUniform1i(index, value));
    }

    void Shader::SetUniform(const char* name, u32 value) const {
        u32 index;
        GLCALL(index = glGetUniformLocation(m_Handle, name));
        GLCALL(glUniform1ui(index, value));
    }

    void Shader::SetUniform(const char* name, f32 value) const {
        u32 index;
        GLCALL(index = glGetUniformLocation(m_Handle, name));
        GLCALL(glUniform1f(index, value));
    }



    void Shader::SetUniform(const char* name, const Math::Vec2& vec) const {
        u32 index;
        GLCALL(index = glGetUniformLocation(m_Handle, name));
        GLCALL(glUniform2f(index, vec.x, vec.y));
    }

    void Shader::SetUniform(const char* name, const Math::Vec3& vec) const {
        u32 index;
        GLCALL(index = glGetUniformLocation(m_Handle, name));
        GLCALL(glUniform3f(index, vec.x, vec.y, vec.z));
    }

    void Shader::SetUniform(const char* name, const Math::Vec4& vec) const {
        u32 index;
        GLCALL(index = glGetUniformLocation(m_Handle, name));
        GLCALL(glUniform4f(index, vec.x, vec.y, vec.z, vec.w));
    }

    void Shader::SetUniform(const char* name, const Math::Mat4& mat) const {
        u32 index;
        GLCALL(index = glGetUniformLocation(m_Handle, name));
        GLCALL(glUniformMatrix4fv(index, 1, false, mat.data()));
    }



    void Shader::SetUniform(const char* name, const i32* values, u32 count) const {
        u32 index;
        GLCALL(index = glGetUniformLocation(m_Handle, name));
        GLCALL(glUniform1iv(index, count, values));
    }

    void Shader::SetUniform(const char* name, const u32* values, u32 count) const {
        u32 index;
        GLCALL(index = glGetUniformLocation(m_Handle, name));
        GLCALL(glUniform1uiv(index, count, values));
    }

    void Shader::SetUniform(const char* name, const f32* values, u32 count) const {
        u32 index;
        GLCALL(index = glGetUniformLocation(m_Handle, name));
        GLCALL(glUniform1fv(index, count, values));
    }



    Shader::~Shader() {
        if (m_Handle)
            GLCALL(glDeleteProgram(m_Handle));
    }
}
