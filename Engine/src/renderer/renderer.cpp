#include <algorithm>

#include "../core/fly_gl.h"
#include "../core/log.h"

#include "../window/fly_window.h"
#include "../scene/camera_component.h"
#include "../scene/render_component.h"
#include "../scene/light_component.h"
#include "../resource/resource_manager.h"
#include "text_renderer.h"
#include "renderer.h"

// dummy variables are just for the correct alignment of the struct
struct Light {
    Math::Mat4 lightMat;
    Math::Vec3 position;  f32 dummy0;  // position WS
    Math::Vec3 direction; f32 dummy1; // direction WS
    Math::Vec3 intensity; f32 dummy2;
    f32 outerAngleCos;
    f32 innerAngleCos;
    i32 type; // 0 - point, 1 - spot
    i32 generateShadows = 0;
};

struct LightBatch {
    Light batch[FLY_LIGHT_BATCH_SIZE];
    i32 numLights;
};

namespace Fly {
    Renderer* g_Renderer = nullptr;

    void Renderer::OnWindowResize(i32 width, i32 height) {
        width = Math::Max(width, 1024);
        height = Math::Max(height, 576);
        m_BlurFramebuffers[0].Resize(width, height);
        m_BlurFramebuffers[1].Resize(width, height);
        m_MultisampleFramebuffer.Resize(width, height);
        m_MainFramebuffer.Resize(width, height);
        
        

        if (CameraComponent::s_MainCamera) {
            CameraComponent::s_MainCamera->SetProjection(Math::Perspective(60.0f, (f32)(width)/height, 0.1f, 100.0f));
        }
    }

    void Renderer::SubmitSprite(const Sprite& sprite) {
        m_TextRenderer->Submit(sprite);
    }

    void Renderer::RegisterComponent(RenderComponent* renderComponent) {
        if (renderComponent->IsTransparent()) {
            m_RenderComponentsTransparent.push_back(renderComponent);
        }
        else {
            m_RenderComponentsOpaque.push_back(renderComponent);
        }
    }

    void Renderer::DeregisterComponent(RenderComponent* renderComponent) {
        auto it1 = std::find(m_RenderComponentsTransparent.begin(), m_RenderComponentsTransparent.end(), renderComponent);

        if (it1 != m_RenderComponentsTransparent.end()) {
            std::iter_swap(it1, m_RenderComponentsTransparent.end() - 1);
            m_RenderComponentsTransparent.pop_back();
        }

        auto it2 = std::find(m_RenderComponentsOpaque.begin(), m_RenderComponentsOpaque.end(), renderComponent);

        if (it2 != m_RenderComponentsOpaque.end()) {
            std::iter_swap(it2, m_RenderComponentsOpaque.end() - 1);
            m_RenderComponentsOpaque.pop_back();
        }
    }

    void Renderer::RegisterComponent(LightComponent* lightComponent) {
        u32 index = (u32)lightComponent->type;
        m_LightComponents.push_back(lightComponent);
    }

    void Renderer::DeregisterComponent(LightComponent* lightComponent) {
        u32 index = (u32)lightComponent->type;

        auto it = std::find(m_LightComponents.begin(), m_LightComponents.end(), lightComponent);

        if (it != m_LightComponents.end()) {
            std::iter_swap(it, m_LightComponents.end() - 1);
            m_LightComponents.pop_back();
        }
    }

    void Renderer::MoveToOpaque(RenderComponent* renderComponent) {
        auto it = std::find(m_RenderComponentsTransparent.begin(), m_RenderComponentsTransparent.end(), renderComponent);

        if (it != m_RenderComponentsTransparent.end()) {
            m_RenderComponentsOpaque.push_back(*it);
            std::iter_swap(it, m_RenderComponentsTransparent.end() - 1);
            m_RenderComponentsTransparent.pop_back();
        }
    }

    void Renderer::MoveToTransparent(RenderComponent* renderComponent) {
        auto it = std::find(m_RenderComponentsOpaque.begin(), m_RenderComponentsOpaque.end(), renderComponent);

        if (it != m_RenderComponentsOpaque.end()) {
            m_RenderComponentsTransparent.push_back(*it);
            std::iter_swap(it, m_RenderComponentsOpaque.end() - 1);
            m_RenderComponentsOpaque.pop_back();
        }
    }

    Math::Vec3 Renderer::ScreenToViewSpace(i32 x, i32 y) {
        i32 width = g_Window->GetWidth();
        i32 height = g_Window->GetHeight();

        x = Math::Clamp(x, 0, width);
        y = Math::Clamp(y, 0, height);

        float z;
        Framebuffer::Bind(&m_MainFramebuffer);
	    GLCALL(glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z));
        Framebuffer::Unbind();
	    Math::Vec2 xy = Math::Vec2((f32)x, (f32)y)/Math::Vec2((f32)width, (f32)height)*2.0f - Math::Vec2(1.0f, 1.0f);
        Math::Vec4 ndc = Math::Vec4(xy, z*2.0f - 1.0f, 1.0f);

        Math::Mat4 invProj = Math::Inverse(CameraComponent::s_MainCamera->GetProjection());
        Math::Mat4 invView = Math::Inverse(CameraComponent::s_MainCamera->GetView());
        Math::Vec4 vs = invProj*ndc;
        vs = vs/vs.w; // perspective division

        
        return Math::Vec3(vs);
    }
    
    i32 Renderer::Init() {
        glewExperimental = true;
        if (glewInit() != GLEW_OK) {
            return -1;
        }

        GLCALL(glEnable(GL_CULL_FACE)); // enable backface culling
        GLCALL(glEnable(GL_MULTISAMPLE)); // enable MSAA
        
        //GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));

        m_CameraBuffer.Create(nullptr, 2*sizeof(Math::Mat4), BufferUsage::DynamicDraw);
        m_LightBuffer.Create(nullptr, sizeof(LightBatch), BufferUsage::DynamicDraw);

        m_TextRenderer = new TextRenderer();
        if (m_TextRenderer->Init() != 0) {
            return -1;
        }

        if (m_MultisampleFramebuffer.Create(true) != 0) {
            return -1;
        };

        if (m_MainFramebuffer.Create() != 0) {
            return -1;
        }

        if (m_BlurFramebuffers[0].Create() != 0) {
            return -1;
        }

        if (m_BlurFramebuffers[1].Create() != 0) {
            return -1;
        }

        m_ToneMappingShader = g_ResourceManager->GetResource<Shader>("Assets/Shaders/tone_mapping.glsl");
        if (!m_ToneMappingShader) {
            return -1;
        }

        m_BlurShader = g_ResourceManager->GetResource<Shader>("Assets/Shaders/blur.glsl");
        if (!m_BlurShader) {
            return -1;
        }

        m_ShadowPassShader = g_ResourceManager->GetResource<Shader>("Assets/Shaders/shadow_pass.glsl");
        if (!m_ShadowPassShader) {
            return -1;
        }

        m_NoiseTexture = g_ResourceManager->GetResource<Texture>("Assets/Textures/Noise/noise.png");
        if (!m_NoiseTexture) {
            return -1;
        }

        // tweaking shadow frameBuffer on low level - don't want to waste time on wrappers
        GLCALL(glGenFramebuffers(1, &m_ShadowFramebufferHandle));
        GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFramebufferHandle));
        GLCALL(glDrawBuffer(GL_NONE));
        //GLCALL(glReadBuffer(GL_NONE));
        GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));

        return 0;
    }

    void Renderer::InsertionSortTransparent() {
        Math::Vec3 camPos = CameraComponent::s_MainCamera->transform.GetPosition();

        for (i32 i = 1; i < (i32)m_RenderComponentsTransparent.size(); i++) {
            RenderComponent* key = m_RenderComponentsTransparent[i];
            f64 value = Math::LengthSqr(camPos - m_RenderComponentsTransparent[i]->transform.GetPosition());
            i32 j = i - 1;

            while (j >= 0 && Math::LengthSqr(camPos - m_RenderComponentsTransparent[j]->transform.GetPosition()) < value) {
                m_RenderComponentsTransparent[j + 1] = m_RenderComponentsTransparent[j];
                j = j - 1;
            }
            m_RenderComponentsTransparent[j + 1] = key;
        }
    }

    void Renderer::ShadowPass(const LightComponent* lc) {
        // Attach this light's shadow map and clear depth
        GLCALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, lc->GetShadowMapHandle(), 0));

        GLCALL(glViewport(0, 0, lc->GetShadowMapWidth(), lc->GetShadowMapHeight()));
        GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));


        for (RenderComponent* rc: m_RenderComponentsOpaque) {
            Texture::ActiveTexture(4);
            Texture::Bind(*m_NoiseTexture);
            rc->DrawInternal(*m_ShadowPassShader, lc->GetLightMatrix());
        }

        for (RenderComponent* rc: m_RenderComponentsTransparent) {
            Texture::ActiveTexture(4);
            Texture::Bind(*m_NoiseTexture);
            rc->DrawInternal(*m_ShadowPassShader, lc->GetLightMatrix());
        }
    }

    void Renderer::DrawOpaque() {
        // Light batch info
        u32 numPointLights = (u32)m_LightComponents.size();
        u32 lightBatchSize = 0;
        LightBatch lightBatch; // point light batch

        bool firstPass = true;  // Only first pass should fill the depth buffer

        // Forward rendering, lights are grouped in batches of size FLY_LIGHT_BATCH_SIZE:
        for (const LightComponent* lc: m_LightComponents) {
            if (lightBatchSize == FLY_LIGHT_BATCH_SIZE) {
                // Case - Overflow of light buffer

                lightBatch.numLights = FLY_LIGHT_BATCH_SIZE;
                m_LightBuffer.ChangeData(&lightBatch, sizeof(LightBatch), 0);
                UniformBuffer::BindBufferBase(m_LightBuffer, 1);

                // Flush and reset
                for (RenderComponent* rc: m_RenderComponentsOpaque) {
                    rc->Draw();
                }

                if (firstPass) {
                    // turn on additive blending
                    GLCALL(glEnable(GL_BLEND));
                    GLCALL(glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ZERO));
                    firstPass = false;
                }
                lightBatchSize = 0;
            }

            // copy data into batch
            switch (lc->type) {
                case LightComponent::Type::Directional: {
                    lightBatch.batch[lightBatchSize].direction = -lc->transform.GetForward();
                    lightBatch.batch[lightBatchSize].intensity = lc->intensity;
                    lightBatch.batch[lightBatchSize].type = (i32)lc->type;

                    lightBatch.batch[lightBatchSize].generateShadows = lc->GetGenerateShadows();
                    if (lc->GetGenerateShadows()) {
                       // bind shadowmap to texture slots 8-16
                        Texture::ActiveTexture(8 + lightBatchSize);
                        GLCALL(glBindTexture(GL_TEXTURE_2D, lc->GetShadowMapHandle()));

                        lightBatch.batch[lightBatchSize].lightMat = lc->GetLightMatrix();
                    }
                    break;
                }
                case LightComponent::Type::Point: {
                    lightBatch.batch[lightBatchSize].position = lc->transform.GetPosition();
                    lightBatch.batch[lightBatchSize].intensity = lc->intensity;
                    lightBatch.batch[lightBatchSize].type = (i32)lc->type;
                    lightBatch.batch[lightBatchSize].generateShadows = lc->GetGenerateShadows();
                    // No shadows for point lights
                    break;
                }
                case LightComponent::Type::Spot: {
                    lightBatch.batch[lightBatchSize].position = lc->transform.GetPosition();
                    lightBatch.batch[lightBatchSize].direction = -lc->transform.GetForward();
                    lightBatch.batch[lightBatchSize].intensity = lc->intensity;
                    lightBatch.batch[lightBatchSize].outerAngleCos = lc->GetOuterAngleCos();
                    lightBatch.batch[lightBatchSize].innerAngleCos = lc->GetInnerAngleCos();
                    lightBatch.batch[lightBatchSize].type = (i32)lc->type;
                    lightBatch.batch[lightBatchSize].generateShadows = lc->GetGenerateShadows();

                    if (lc->GetGenerateShadows()) {
                       // bind shadowmap to texture slots 8-16
                        Texture::ActiveTexture(8 + lightBatchSize);
                        GLCALL(glBindTexture(GL_TEXTURE_2D, lc->GetShadowMapHandle()));

                        lightBatch.batch[lightBatchSize].lightMat = lc->GetLightMatrix();
                    }
                    break;
                }
            }
            lightBatchSize++;
        }

        // Flush the rest
        if (lightBatchSize > 0) {
            lightBatch.numLights = lightBatchSize;
            m_LightBuffer.ChangeData(&lightBatch, sizeof(LightBatch), 0);
            UniformBuffer::BindBufferBase(m_LightBuffer, 1);
            for (RenderComponent* rc: m_RenderComponentsOpaque) {
                rc->Draw();
            }
            lightBatchSize = 0;
        }
    }

    void Renderer::DrawTransparent() {
                // Light batch info
        u32 numPointLights = (u32)m_LightComponents.size();
        u32 lightBatchSize = 0;
        LightBatch lightBatch; // point light batch

        bool firstPass = true;  // Only first pass should fill the depth buffer

        // Forward rendering, lights are grouped in batches of size FLY_LIGHT_BATCH_SIZE:
        for (const LightComponent* lc: m_LightComponents) {
            if (lightBatchSize == FLY_LIGHT_BATCH_SIZE) {
                // Case - Overflow of light buffer

                lightBatch.numLights = FLY_LIGHT_BATCH_SIZE;
                m_LightBuffer.ChangeData(&lightBatch, sizeof(LightBatch), 0);
                UniformBuffer::BindBufferBase(m_LightBuffer, 1);

                // Flush and reset
                GLCALL(glEnable(GL_BLEND));
                GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

                for (RenderComponent* rc: m_RenderComponentsTransparent) {
                    rc->Draw();
                }

                if (firstPass) {
                    GLCALL(glEnable(GL_BLEND));
                    GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE));
                    firstPass = false;
                }
                lightBatchSize = 0;
            }

            // copy data into batch
            switch (lc->type) {
                case LightComponent::Type::Directional: {
                    lightBatch.batch[lightBatchSize].direction = -lc->transform.GetForward();
                    lightBatch.batch[lightBatchSize].intensity = lc->intensity;
                    lightBatch.batch[lightBatchSize].type = (i32)lc->type;

                    lightBatch.batch[lightBatchSize].generateShadows = lc->GetGenerateShadows();
                    if (lc->GetGenerateShadows()) {
                       // bind shadowmap to texture slots 8-16
                        Texture::ActiveTexture(8 + lightBatchSize);
                        GLCALL(glBindTexture(GL_TEXTURE_2D, lc->GetShadowMapHandle()));

                        lightBatch.batch[lightBatchSize].lightMat = lc->GetLightMatrix();
                    }
                    break;
                }
                case LightComponent::Type::Point: {
                    lightBatch.batch[lightBatchSize].position = lc->transform.GetPosition();
                    lightBatch.batch[lightBatchSize].intensity = lc->intensity;
                    lightBatch.batch[lightBatchSize].type = (i32)lc->type;
                    lightBatch.batch[lightBatchSize].generateShadows = lc->GetGenerateShadows();
                    // No shadows for point lights
                    break;
                }
                case LightComponent::Type::Spot: {
                    lightBatch.batch[lightBatchSize].position = lc->transform.GetPosition();
                    lightBatch.batch[lightBatchSize].direction = -lc->transform.GetForward();
                    lightBatch.batch[lightBatchSize].intensity = lc->intensity;
                    lightBatch.batch[lightBatchSize].outerAngleCos = lc->GetOuterAngleCos();
                    lightBatch.batch[lightBatchSize].innerAngleCos = lc->GetInnerAngleCos();
                    lightBatch.batch[lightBatchSize].type = (i32)lc->type;
                    lightBatch.batch[lightBatchSize].generateShadows = lc->GetGenerateShadows();

                    if (lc->GetGenerateShadows()) {
                       // bind shadowmap to texture slots 8-16
                        Texture::ActiveTexture(8 + lightBatchSize);
                        GLCALL(glBindTexture(GL_TEXTURE_2D, lc->GetShadowMapHandle()));

                        lightBatch.batch[lightBatchSize].lightMat = lc->GetLightMatrix();
                    }
                    break;
                }
            }
            lightBatchSize++;
        }

        // Flush the rest
        if (lightBatchSize > 0) {
            lightBatch.numLights = lightBatchSize;
            m_LightBuffer.ChangeData(&lightBatch, sizeof(LightBatch), 0);
            UniformBuffer::BindBufferBase(m_LightBuffer, 1);

            if (firstPass) {
                GLCALL(glEnable(GL_BLEND));
                GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
            }
            for (RenderComponent* rc: m_RenderComponentsTransparent) {
                rc->Draw();
            }
            lightBatchSize = 0;
        }

        // reset gl state to default
        GLCALL(glDisable(GL_BLEND));
        GLCALL(glDepthFunc(GL_LEQUAL));
        GLCALL(glDepthMask(GL_TRUE));
    }


    void Renderer::ForwardRendering() {
        InsertionSortTransparent();
        GLCALL(glEnable(GL_DEPTH_TEST));

        // Shadow Pass (creating shadowMaps)
        GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFramebufferHandle));
        for (const LightComponent* lc: m_LightComponents) {
            if (lc->GetGenerateShadows()) {
                ShadowPass(lc);
            }
        }

        // Bind multisamlpe framebuffer
        Framebuffer::Bind(&m_MultisampleFramebuffer);
        GLCALL(glViewport(0, 0, g_Window->GetWidth(), g_Window->GetHeight()));
        GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        // Updating camera data
        Math::Mat4 matrices[2];
        matrices[0] = CameraComponent::s_MainCamera->GetProjection();
        matrices[1] = CameraComponent::s_MainCamera->GetView();
        m_CameraBuffer.ChangeData(&matrices, 2*sizeof(Math::Mat4), 0);
        UniformBuffer::BindBufferBase(m_CameraBuffer, 0);

        DrawOpaque();
        DrawTransparent();

        // Blit from multisample to normal framebuffer in order to allow access to color attachment
        i32 width = g_Window->GetWidth();
        i32 height = g_Window->GetHeight();
        Framebuffer::Bind(&m_MultisampleFramebuffer, Framebuffer::BindMode::Read);
        Framebuffer::Bind(&m_MainFramebuffer, Framebuffer::BindMode::Write);

        GLCALL(glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST));
        GLCALL(glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST));
    }

    void Renderer::PostProcessing() {
        i32 width = g_Window->GetWidth();
        i32 height = g_Window->GetHeight();
        
        VertexBuffer::Unbind();
        Texture::ActiveTexture(10);
        
        Framebuffer::Bind(&m_MainFramebuffer, Framebuffer::BindMode::Read);
        Framebuffer::Bind(&m_BlurFramebuffers[1], Framebuffer::BindMode::Write);
        GLCALL(glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST));

        if (m_Blur) {
            Shader::Use(*m_BlurShader);
            for (int i = 0; i < 3; i++) {
                Framebuffer::Bind(&m_BlurFramebuffers[0]);
                GLCALL(glClear(GL_DEPTH_BUFFER_BIT));
                Texture::ActiveTexture(10);
                GLCALL(glBindTexture(GL_TEXTURE_2D, m_BlurFramebuffers[1].GetColorAttachmentHandle()));
                m_BlurShader->SetUniform("u_IsHorizontal", true);
                GLCALL(glDrawArrays(GL_POINTS, 0, 1));

                Framebuffer::Bind(&m_BlurFramebuffers[1]);
                GLCALL(glClear(GL_DEPTH_BUFFER_BIT));
                Texture::ActiveTexture(10);
                GLCALL(glBindTexture(GL_TEXTURE_2D, m_BlurFramebuffers[0].GetColorAttachmentHandle()));
                m_BlurShader->SetUniform("u_IsHorizontal", false);
                GLCALL(glDrawArrays(GL_POINTS, 0, 1));
            }
        }

        // Tone mapping
        Framebuffer::Unbind();
        GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GLCALL(glBindTexture(GL_TEXTURE_2D, m_BlurFramebuffers[1].GetColorAttachmentHandle()));
        Shader::Use(*m_ToneMappingShader);
        GLCALL(glDrawArrays(GL_POINTS, 0, 1));
    }

    void Renderer::UI() {
        m_TextRenderer->Draw();
    }

    void Renderer::Draw() {
        ForwardRendering();
        PostProcessing();
        UI();
    }

    Renderer::~Renderer() {
        m_MultisampleFramebuffer.Destroy();
        m_MainFramebuffer.Destroy();
        delete m_TextRenderer;
        m_CameraBuffer.Destroy();
    }
}
