#ifndef FLY_SHADER_H
#define FLY_SHADER_H

#include "../math/mat.h"
#include "../resource/resource.h"

namespace Fly {
    class Shader: public Resource {
        friend class Loader;
    public:
        void SetUniform(const char* name, i32 value) const;
        void SetUniform(const char* name, u32 value) const;
        void SetUniform(const char* name, f32 value) const;

        void SetUniform(const char* name, const Math::Vec2& vec) const;
        void SetUniform(const char* name, const Math::Vec3& vec) const;
        void SetUniform(const char* name, const Math::Vec4& vec) const;
        void SetUniform(const char* name, const Math::Mat4& mat) const;

        void SetUniform(const char* name, const i32* array, u32 count) const;
        void SetUniform(const char* name, const u32* array, u32 count) const;
        void SetUniform(const char* name, const f32* array, u32 count) const;
        ~Shader();

        static void Use(const Shader& shader);
    private:
        u32 m_Handle = 0;
    };
}

#endif /* End of FLY_SHADER_H */
