#include "../math/general.h"
#include "../core/fly_gl.h"
#include "texture.h"

namespace Fly {
    void Texture::Bind(const Texture& texture) {
        GLCALL(glBindTexture(GL_TEXTURE_2D, texture.m_Handle));
    }

    void Texture::ActiveTexture(u32 slot) {
        GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
    }
    
    void Texture::SetWrapMode(Wrap wrapMode) {
        GLCALL(glBindTexture(GL_TEXTURE_2D, m_Handle));
        switch(m_WrapMode) {
            case Wrap::Repeat: {
                GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
                GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
                break;
            }
            case Wrap::Mirrored: {
                GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
                GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));
                break;
            }
            case Wrap::Clamp: {
                GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
                GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
                break;
            }
            m_WrapMode = wrapMode;
        }
    }

    void Texture::SetFilteringMode(Filtering filtering) {
        GLCALL(glBindTexture(GL_TEXTURE_2D, m_Handle));
        switch(filtering) {
            case Filtering::Nearest: {
                GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
                GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
                break;
            }
            case Filtering::Linear: {
                GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
                GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
                break;
            }
            case Filtering::Trilinear: {
                GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
                GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            }
        }
        m_Filtering = filtering;
    }

    void Texture::GenerateMipmaps() {
        GLCALL(glBindTexture(GL_TEXTURE_2D, m_Handle));
        //GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

        if (GLEW_EXT_texture_filter_anisotropic) {
            float max;
            GLCALL(glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max));
            float amount = Math::Min(4.0f, max);

            GLCALL(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, amount));
        }
    }

    Texture* Texture::CreateBlackTexture() {
        Texture* tex = new Texture();

        GLCALL(glGenTextures(1, &tex->m_Handle));
        GLCALL(glBindTexture(GL_TEXTURE_2D, tex->m_Handle));
        u32 blackPixel = 0x000000ff;
        GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &blackPixel));

        return tex;
    }
}
