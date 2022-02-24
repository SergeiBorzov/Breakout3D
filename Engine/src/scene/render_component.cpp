#include "../core/log.h"
#include "../core/fly_gl.h"
#include "../renderer/renderer.h"
#include "render_component.h"

namespace Fly {
    const u32 RenderComponent::s_ComponentID = ComponentID<RenderComponent>::value();

    RenderComponent::RenderComponent(Fly::Entity& entity):
        Component(entity),
        mesh(nullptr)
    {
        g_Renderer->RegisterComponent(this);
    }

    RenderComponent::RenderComponent(Fly::Entity& entity, const Mesh* mesh, const Material* material):
        Component(entity),
        mesh(mesh)
    {
        materials.push_back(material);
        g_Renderer->RegisterComponent(this);
    }

    void RenderComponent::SetTransparency(bool value) {
        if (isTransparent != value) {
            isTransparent = value;
            if (value) {
                g_Renderer->MoveToTransparent(this);
            }
            else {
                g_Renderer->MoveToOpaque(this);
            }
        }
    }

    void RenderComponent::Draw() const {
        if (!entity.IsActive()) {
            return;
        }

        if (mesh && mesh->m_Submeshes.size() == materials.size()) {
            for (int i = 0; i < mesh->m_Submeshes.size(); i++) {
                materials[i]->SetProperties(opaqueness, entity.transform.GetModelMatrix());
                VertexArray::Bind(mesh->m_Submeshes[i].vertexArray);
                IndexBuffer::Bind(mesh->m_Submeshes[i].indexBuffer);

                u32 count = mesh->m_Submeshes[i].GetIndexCount();
                GLCALL(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0));
            }
        }
    }

    void RenderComponent::DrawInternal(const Shader& shader, const Math::Mat4& projectionView) const {
        if (!entity.IsActive()) {
            return;
        }
        Shader::Use(shader);
        shader.SetUniform("u_ProjectionView", projectionView);
        if (mesh) {
            for (int i = 0; i < mesh->m_Submeshes.size(); i++) {
                shader.SetUniform("u_Model", transform.GetModelMatrix());
                PBRMaterial* mat = (PBRMaterial*)materials[i];
                shader.SetUniform("u_Tiling", mat->tiling);
                shader.SetUniform("u_Threshold", 1.0f - mat->threshold);

                VertexArray::Bind(mesh->m_Submeshes[i].vertexArray);
                IndexBuffer::Bind(mesh->m_Submeshes[i].indexBuffer);

                u32 count = mesh->m_Submeshes[i].GetIndexCount();
                GLCALL(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0));
            }
        }
    }

    RenderComponent::~RenderComponent() {
        g_Renderer->DeregisterComponent(this);
    }
}
