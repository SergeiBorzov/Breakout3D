#include "../core/fly_gl.h"
#include "../core/log.h"
#include "../window/fly_window.h"
#include "framebuffer.h"

namespace Fly {
    void Framebuffer::Bind(const Framebuffer* fb, BindMode mode) {
        switch (mode) {
            case BindMode::ReadWrite: {
                if (fb) {
                    GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, fb->m_Handle));
                }
                else {
                    GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
                }
                
                break;
            }
            case BindMode::Read: {
                if (fb) {
                    GLCALL(glBindFramebuffer(GL_READ_FRAMEBUFFER, fb->m_Handle));
                }
                else {
                    GLCALL(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));
                }
                break;
            }
            case BindMode::Write: {
                if (fb) {
                    GLCALL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb->m_Handle));
                }
                else {
                    GLCALL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
                }
                break;
            }
        }
    }

    void Framebuffer::Unbind() {
        GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    i32 Framebuffer::Create(bool multisampled) {
        m_Multisampled = multisampled;
        i32 width = g_Window->GetWidth();
        i32 height = g_Window->GetHeight();
        GLCALL(glGenFramebuffers(1, &m_Handle));
        GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, m_Handle));
        GLCALL(glGenTextures(1, &m_ColorHandle));
        GLCALL(glGenTextures(1, &m_DepthStencilHandle));

        if (!multisampled) {
            GLCALL(glBindTexture(GL_TEXTURE_2D, m_ColorHandle));
            GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL));
            GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
            GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));
            GLCALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorHandle, 0));

            GLCALL(glBindTexture(GL_TEXTURE_2D, m_DepthStencilHandle));
            GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL));
            GLCALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthStencilHandle, 0));
        }
        else {
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_ColorHandle);
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA32F, width, height, GL_TRUE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_ColorHandle, 0); 

            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_DepthStencilHandle);
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_DEPTH24_STENCIL8, width, height, GL_TRUE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, m_DepthStencilHandle, 0); 
        }
        
        if  (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            return -1;
        }

        GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        return 0;
    }

    void Framebuffer::Resize(i32 width, i32 height) {
        if (!m_Multisampled) {
            GLCALL(glBindTexture(GL_TEXTURE_2D, m_ColorHandle));
            GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL));
            GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
            GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));
            //GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            //GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            //GLCALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorHandle, 0));

            GLCALL(glBindTexture(GL_TEXTURE_2D, m_DepthStencilHandle));
            GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL));
            //GLCALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthStencilHandle, 0));
        }
        else {
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_ColorHandle);
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA32F, width, height, GL_TRUE);
            //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_ColorHandle, 0); 

            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_DepthStencilHandle);
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_DEPTH24_STENCIL8, width, height, GL_TRUE);
            //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, m_DepthStencilHandle, 0); 
        }
    }

    void Framebuffer::Destroy() {
        if (m_Handle) {
            GLCALL(glDeleteFramebuffers(1, &m_Handle));
        }
    }
}