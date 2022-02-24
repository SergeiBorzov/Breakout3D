#ifndef FLY_MESH_H
#define FLY_MESH_H

#include <vector>

#include "../math/general.h"
#include "../resource/resource.h"
#include "vertex_array.h"

namespace Fly {
    class Submesh {
        friend class Loader;
        friend class Mesh;
    public:
        u32 GetVertexCount() const { return m_VertexCount; }
        u32 GetIndexCount() const { return m_IndexCount; }
        VertexArray vertexArray;
        VertexBuffer vertexBuffer;
        IndexBuffer indexBuffer;
    private:
        Submesh(const std::vector<f32>& vertices, const std::vector<u32>& indices,
                const std::initializer_list<Attribute>& attributes);
        u32 m_VertexCount = 0;
        u32 m_IndexCount = 0;        
    };
    
    class Mesh: public Resource {
        friend class Loader;
        friend class RenderComponent;
    public:
        virtual ~Mesh();

        static void CalculateTangents(const std::vector<Math::Vec3>& positions,
                                      const std::vector<Math::Vec3>& normals,
                                      const std::vector<Math::Vec2>& uvs,
                                      const std::vector<u32>& indices,
                                      std::vector<Math::Vec3>& tangents);

        static Mesh* CreateSphere(f32 radius, i32 segments, i32 rings);
        static Mesh* CreateBox(f32 rx, f32 ry, f32 rz);
        static Mesh* CreateCylinderBrick(f32 R, f32 r, f32 h, f32 angle, i32 segments); // angle is in degrees
    private:
        Mesh(const char* name, const std::vector<Submesh>& submeshes): m_Submeshes(submeshes) {
            m_Name = name;
        }

        std::vector<Submesh> m_Submeshes;
    };
}

#endif /* End of FLY_MESH_H */
