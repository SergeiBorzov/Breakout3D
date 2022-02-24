#ifndef FLY_RENDERER_H
#define FLY_RENDERER_H

#include <vector>

#include "../core/platform.h"
#include "shader.h"
#include "framebuffer.h"
#include "buffers.h"

#define FLY_LIGHT_BATCH_SIZE 8

namespace Fly {
    struct Sprite;
    class TextRenderer;
    class RenderComponent;
    class LightComponent;
    class Texture;

    class Renderer {
    public:
        i32 Init();
        void Draw();

        void RegisterComponent(RenderComponent* renderComponent);
        void DeregisterComponent(RenderComponent* renderComponent);

        void MoveToTransparent(RenderComponent* renderComponent);
        void MoveToOpaque(RenderComponent* RenderComponent);

        void RegisterComponent(LightComponent* lightComponent);
        void DeregisterComponent(LightComponent* lightComponent);

        Math::Vec3 ScreenToViewSpace(i32 x, i32 y);

        void SubmitSprite(const Sprite& sprite);
        inline void SetBlur(bool value) { m_Blur = value; }
        void OnWindowResize(i32 width, i32 height);

        ~Renderer();
    private:
        void InsertionSortTransparent();
        void ShadowPass(const LightComponent* lc);
        void ForwardRendering();
        void DrawOpaque();
        void DrawTransparent();

        void PostProcessing();
        void UI();

        std::vector<RenderComponent*> m_RenderComponentsOpaque;
        std::vector<RenderComponent*> m_RenderComponentsTransparent;

        std::vector<LightComponent*> m_LightComponents;
        TextRenderer* m_TextRenderer = nullptr; 
        UniformBuffer m_CameraBuffer;
        UniformBuffer m_LightBuffer;

        Framebuffer m_BlurFramebuffers[2];
        Framebuffer m_MultisampleFramebuffer;
        Framebuffer m_MainFramebuffer;

        Texture* m_NoiseTexture = nullptr;
        Shader* m_ShadowPassShader = nullptr;
        Shader* m_ToneMappingShader = nullptr;
        Shader* m_BlurShader = nullptr;
        u32 m_ShadowFramebufferHandle = 0;

        bool m_Blur = false;
    };

    FLY_API extern Renderer* g_Renderer;
}

#endif /* End of FLY_RENDERER_H */
