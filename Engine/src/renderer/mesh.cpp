#include "../core/log.h"
#include "mesh.h"

namespace Fly {
    Submesh::Submesh(const std::vector<f32>& vertices, const std::vector<u32>& indices,
                     const std::initializer_list<Attribute>& attributes) {
        m_VertexCount = (u32)vertices.size();
        m_IndexCount = (u32)indices.size();
        vertexBuffer.Create(vertices.data(), vertices.size()*sizeof(f32), BufferUsage::StaticDraw);
        indexBuffer.Create(indices.data(), indices.size()*sizeof(u32), BufferUsage::StaticDraw);
        vertexArray.SetAttributes(vertexBuffer, attributes);
    }

    Mesh::~Mesh() {
        for (int i = 0; i < m_Submeshes.size(); i++) {
            m_Submeshes[i].vertexArray.Destroy();
            m_Submeshes[i].indexBuffer.Destroy();
            m_Submeshes[i].vertexBuffer.Destroy();
        }
    }


    void Mesh::CalculateTangents(const std::vector<Math::Vec3>& positions,
                                 const std::vector<Math::Vec3>& normals,
                                 const std::vector<Math::Vec2>& uvs,
                                 const std::vector<u32>& indices,
                                 std::vector<Math::Vec3>& tangents) {
        tangents.clear();
        tangents.resize(normals.size());

        std::vector<Math::Vec3> bitangents;
        bitangents.resize(normals.size());

        for (u64 i = 0; i < indices.size() / 3; i++) {
            const Math::Vec3& p0 = positions[indices[3*i + 0]];
            const Math::Vec3& p1 = positions[indices[3*i + 1]];
            const Math::Vec3& p2 = positions[indices[3*i + 2]];

            const Math::Vec2& w0 = uvs[indices[3*i + 0]];
            const Math::Vec2& w1 = uvs[indices[3*i + 1]];
            const Math::Vec2& w2 = uvs[indices[3*i + 2]];

            Math::Vec3 e1 = p1 - p0;
            Math::Vec3 e2 = p2 - p0;
            f32 x1 = w1.x - w0.x;
            f32 x2 = w2.x - w0.x;
            f32 y1 = w1.y - w0.y;
            f32 y2 = w2.y - w0.y;

            f32 r = 1.0f/(x1*y2 - x2*y1);
            Math::Vec3 t = (e1*y2 - e2*y1)*r;
            Math::Vec3 b = (e2*x1 - e1*x2)*r;

            tangents[indices[3*i + 0]] += t;
            tangents[indices[3*i + 1]] += t;
            tangents[indices[3*i + 2]] += t;

            bitangents[indices[3*i + 0]] += b;
            bitangents[indices[3*i + 1]] += b;
            bitangents[indices[3*i + 2]] += b;
        }

        for (u64 i = 0; i < positions.size(); i++) {
            const Math::Vec3& t = tangents[i];
            const Math::Vec3& b = bitangents[i];
            const Math::Vec3& n = normals[i];
            
            Math::Vec3 tangent = Math::Normalize(t - Math::Dot(t, n)*n);
            f32 sign = (Math::Dot(Math::Cross(t, b), n) > 0.0f) ? 1.0f: -1.0f;
            tangents[i] = sign*tangent;
        }
    }

    Mesh* Mesh::CreateBox(f32 rx, f32 ry, f32 rz) {
        std::vector<Math::Vec3> positions;
        std::vector<Math::Vec3> normals;
        std::vector<Math::Vec3> tangents;
        std::vector<Math::Vec2> uvs;
        std::vector<u32> indices;

        // Front face:
        {
            //0
            positions.push_back({-rx, ry, rz});
            normals.push_back({0.0f, 0.0f, 1.0f});
            uvs.push_back({0.0f, 1.0f});
            //1
            positions.push_back({-rx, -ry, rz});
            normals.push_back({0.0f, 0.0f, 1.0f});
            uvs.push_back({0.0f, 0.0f});
             //2
            positions.push_back({rx, ry, rz});
            normals.push_back({0.0f, 0.0f, 1.0f});
            uvs.push_back({1.0f, 1.0f});
            //3
            positions.push_back({rx, -ry, rz});
            normals.push_back({0.0f, 0.0f, 1.0f});
            uvs.push_back({1.0f, 0.0f});

            indices.push_back(0);
            indices.push_back(1);
            indices.push_back(2);
            indices.push_back(1);
            indices.push_back(3);
            indices.push_back(2);
        }
        
        // Back face:
        {
            //4
            positions.push_back({rx, ry, -rz});
            normals.push_back({0.0f, 0.0f, -1.0f});
            uvs.push_back({0.0f, 1.0f});
            //5
            positions.push_back({rx, -ry, -rz});
            normals.push_back({0.0f, 0.0f, -1.0f});
            uvs.push_back({0.0f, 0.0f});
            //6
            positions.push_back({-rx, ry, -rz});
            normals.push_back({0.0f, 0.0f, -1.0f});
            uvs.push_back({1.0f, 1.0f});
            //7
            positions.push_back({-rx, -ry, -rz});
            normals.push_back({0.0f, 0.0f, -1.0f});
            uvs.push_back({1.0f, 0.0f});

            indices.push_back(4);
            indices.push_back(5);
            indices.push_back(6);
            indices.push_back(5);
            indices.push_back(7);
            indices.push_back(6);
        }

        // Left face:
        {
            //8
            positions.push_back({-rx, ry, -rz});
            normals.push_back({-1.0f, 0.0f, 0.0f});
            uvs.push_back({0.0f, 1.0f});
            //9
            positions.push_back({-rx, -ry, -rz});
            normals.push_back({-1.0f, 0.0f, 0.0f});
            uvs.push_back({0.0f, 0.0f});
            //10
            positions.push_back({-rx, ry, rz});
            normals.push_back({-1.0f, 0.0f, 0.0f});
            uvs.push_back({1.0f, 1.0f});
            //11
            positions.push_back({-rx, -ry, rz});
            normals.push_back({-1.0f, 0.0f, 0.0f});
            uvs.push_back({1.0f, 0.0f});

            indices.push_back(8);
            indices.push_back(9);
            indices.push_back(10);
            indices.push_back(9);
            indices.push_back(11);
            indices.push_back(10);
        }

        // Right face:
        {
            //12
            positions.push_back({rx, ry, rz});
            normals.push_back({1.0f, 0.0f, 0.0f});
            uvs.push_back({0.0f, 1.0f});
            //13
            positions.push_back({rx, -ry, rz});
            normals.push_back({1.0f, 0.0f, 0.0f});
            uvs.push_back({0.0f, 0.0f});
            //14
            positions.push_back({rx, ry, -rz});
            normals.push_back({1.0f, 0.0f, 0.0f});
            uvs.push_back({1.0f, 1.0f});
            //15
            positions.push_back({rx, -ry, -rz});
            normals.push_back({1.0f, 0.0f, 0.0f});
            uvs.push_back({1.0f, 0.0f});

            indices.push_back(12);
            indices.push_back(13);
            indices.push_back(14);
            indices.push_back(13);
            indices.push_back(15);
            indices.push_back(14);
        }

        // Top face:
        {
            //16
            positions.push_back({-rx, ry, -rz});
            normals.push_back({0.0f, 1.0f, 0.0f});
            uvs.push_back({0.0f, 1.0f});
            //17
            positions.push_back({-rx, ry, rz});
            normals.push_back({0.0f, 1.0f, 0.0f});
            uvs.push_back({0.0f, 0.0f});
            //18
            positions.push_back({rx, ry, -rz});
            normals.push_back({0.0f, 1.0f, 0.0f});
            uvs.push_back({1.0f, 1.0f});
            //19
            positions.push_back({rx, ry, rz});
            normals.push_back({0.0f, 1.0f, 0.0f});
            uvs.push_back({1.0f, 0.0f});

            indices.push_back(16);
            indices.push_back(17);
            indices.push_back(18);
            indices.push_back(17);
            indices.push_back(19);
            indices.push_back(18);
        }

        // Bottom face:
        {
            //20
            positions.push_back({rx, -ry, rz});
            normals.push_back({0.0f, -1.0f, 0.0f});
            uvs.push_back({0.0f, 1.0f});
            //21
            positions.push_back({rx, -ry, -rz});
            normals.push_back({0.0f, -1.0f, 0.0f});
            uvs.push_back({0.0f, 0.0f});
            //22
            positions.push_back({-rx, -ry, rz});
            normals.push_back({0.0f, -1.0f, 0.0f});
            uvs.push_back({1.0f, 1.0f});
            //23
            positions.push_back({-rx, -ry, -rz});
            normals.push_back({0.0f, -1.0f, 0.0f});
            uvs.push_back({1.0f, 0.0f});

            indices.push_back(21);
            indices.push_back(20);
            indices.push_back(22);
            indices.push_back(21);
            indices.push_back(22);
            indices.push_back(23);
        }

        CalculateTangents(positions, normals, uvs, indices, tangents);
        std::vector<f32> vertices;

        for (u64 i = 0; i < positions.size(); i++) {
            vertices.push_back(positions[i].x);
            vertices.push_back(positions[i].y);
            vertices.push_back(positions[i].z);
            vertices.push_back(normals[i].x);
            vertices.push_back(normals[i].y);
            vertices.push_back(normals[i].z);
            vertices.push_back(tangents[i].x);
            vertices.push_back(tangents[i].y);
            vertices.push_back(tangents[i].z);
            vertices.push_back(uvs[i].x);
            vertices.push_back(uvs[i].y);
        }

        std::vector<Submesh> submeshes;
        submeshes.push_back(Submesh(vertices, indices, 
                                    {{Fly::Attribute::Vec3}, 
                                     {Fly::Attribute::Vec3},
                                     {Fly::Attribute::Vec3}, 
                                     {Fly::Attribute::Vec2}}));

        Mesh* mesh = new Mesh("Box", submeshes);
        return mesh;
    }

    Mesh* Mesh::CreateCylinderBrick(f32 R, f32 r, f32 h, f32 angle, i32 segments) {
        std::vector<Math::Vec3> positions;
        std::vector<Math::Vec3> normals;
        std::vector<Math::Vec3> tangents;
        std::vector<Math::Vec2> uvs;
        std::vector<u32> indices;

        segments = Math::Max(segments, 3);

        f32 midR = 0.5f*(R + r);
        Math::Vec3 origin = Math::Vec3(midR*Math::Cos(Math::Radians(angle*0.5f)), 0.0f, midR*Math::Sin(Math::Radians(angle*0.5f)));

        f32 deltaAngle = angle/(segments - 1);

        f32 cosTheta = Math::Cos(Math::Radians(90.0f - angle/2)); 
        f32 sinTheta = Math::Sin(Math::Radians(90.0f - angle/2));

        //Math::Mat3 rotation = Math::Mat3();
        Math::Mat3 rotation = Math::Mat3({cosTheta, 0.0f,  sinTheta},
                                         {0.0f, 1.0f, 0.0f},
                                         {-sinTheta, 0.0f, cosTheta});
                                         
        // Fill inner points
        for (i32 i = 0; i < segments; i++) {
            Math::Vec3 innerPointLow = 
                Math::Vec3(r*Math::Cos(Math::Radians(i*deltaAngle)), -0.5f*h, r*Math::Sin(Math::Radians(i*deltaAngle)));
            Math::Vec3 innerPointHigh = 
                Math::Vec3(r*Math::Cos(Math::Radians(i*deltaAngle)), 0.5f*h, r*Math::Sin(Math::Radians(i*deltaAngle)));
            
            positions.push_back(rotation*(innerPointLow - origin));
            normals.push_back(Math::Normalize(rotation*-innerPointLow));
            uvs.push_back(Math::Vec2(i*deltaAngle/angle, 0.0f));
            positions.push_back(rotation*(innerPointHigh - origin));
            normals.push_back(Math::Normalize(rotation*-innerPointHigh));
            uvs.push_back(Math::Vec2(i*deltaAngle/angle, 1.0f));            
        }

        // Fill outer points
        for (i32 i = 0; i < segments; i++) {
            Math::Vec3 outerPointLow = 
                Math::Vec3(R*Math::Cos(Math::Radians(i*deltaAngle)), -0.5f*h, R*Math::Sin(Math::Radians(i*deltaAngle)));
            Math::Vec3 outerPointHigh = 
                Math::Vec3(R*Math::Cos(Math::Radians(i*deltaAngle)),  0.5f*h, R*Math::Sin(Math::Radians(i*deltaAngle)));
            positions.push_back(rotation*(outerPointLow - origin));
            normals.push_back(Math::Normalize(rotation*outerPointLow));
            uvs.push_back(Math::Vec2(i*deltaAngle/angle, 0.0f));
            positions.push_back(rotation*(outerPointHigh - origin));
            normals.push_back(Math::Normalize(rotation*outerPointHigh));
            uvs.push_back(Math::Vec2(i*deltaAngle/angle, 1.0f));
        }

        // Fill top point
        for (i32 i = 0; i < segments; i++) {
            Math::Vec3 topPointLow = 
                Math::Vec3(r*Math::Cos(Math::Radians(i*deltaAngle)),  0.5f*h, r*Math::Sin(Math::Radians(i*deltaAngle)));
            Math::Vec3 topPointHigh = 
                Math::Vec3(R*Math::Cos(Math::Radians(i*deltaAngle)),  0.5f*h, R*Math::Sin(Math::Radians(i*deltaAngle)));
            positions.push_back(rotation*(topPointLow - origin));
            normals.push_back({0.0f, 1.0f, 0.0f});
            uvs.push_back(Math::Vec2(i*deltaAngle/angle, 0.0f));
            positions.push_back(rotation*(topPointHigh - origin));
            normals.push_back({0.0f, 1.0f, 0.0f});
            uvs.push_back(Math::Vec2(i*deltaAngle/angle, Math::Min((R - r) / h, 1.0f)));
        }

        // Fill bot point
        for (i32 i = 0; i < segments; i++) {
            Math::Vec3 botPointLow = 
                Math::Vec3(r*Math::Cos(Math::Radians(i*deltaAngle)),  -0.5f*h, r*Math::Sin(Math::Radians(i*deltaAngle)));
            Math::Vec3 botPointHigh = 
                Math::Vec3(R*Math::Cos(Math::Radians(i*deltaAngle)),  -0.5f*h, R*Math::Sin(Math::Radians(i*deltaAngle)));
            positions.push_back(rotation*(botPointLow - origin));
            normals.push_back({0.0f, -1.0f, 0.0f});
            uvs.push_back(Math::Vec2(i*deltaAngle/angle, 0.0f));
            positions.push_back(rotation*(botPointHigh - origin));
            normals.push_back({0.0f, -1.0f, 0.0f});
            uvs.push_back(Math::Vec2(i*deltaAngle/angle, Math::Min((R - r) / h, 1.0f)));
        }

        // Create inner face
        for (u32 i = 0; i < positions.size()/4 - 2; i += 2) {
            u32 innerlow1 = i + 0;
            u32 innerlow2 = i + 2;
            u32 innerhigh1 = i + 1;
            u32 innerhigh2 = i + 3;
            indices.push_back(innerlow1);
            indices.push_back(innerlow2);
            indices.push_back(innerhigh1);
            indices.push_back(innerhigh1);
            indices.push_back(innerlow2);
            indices.push_back(innerhigh2);


            u32 outerlow1 = (u32)positions.size()/4 + i + 0;
            u32 outerlow2 = (u32)positions.size()/4 + i + 2;
            u32 outerhigh1 = (u32)positions.size()/4 + i + 1;
            u32 outerhigh2 = (u32)positions.size()/4 + i + 3;
            indices.push_back(outerlow2);
            indices.push_back(outerlow1);
            indices.push_back(outerhigh1);
            indices.push_back(outerlow2);
            indices.push_back(outerhigh1);
            indices.push_back(outerhigh2);

            u32 toplow1 = (u32)positions.size()/2 + i + 0;
            u32 toplow2 = (u32)positions.size()/2 + i + 2;
            u32 tophigh1 = (u32)positions.size()/2 + i + 1;
            u32 tophigh2 = (u32)positions.size()/2 + i + 3;
            indices.push_back(toplow2);
            indices.push_back(tophigh1);
            indices.push_back(toplow1);
            indices.push_back(toplow2);
            indices.push_back(tophigh2);
            indices.push_back(tophigh1);

            u32 botlow1 = 3*(u32)positions.size()/4 + i + 0;
            u32 botlow2 = 3*(u32)positions.size()/4 + i + 2;
            u32 bothigh1 = 3*(u32)positions.size()/4 + i + 1;
            u32 bothigh2 = 3*(u32)positions.size()/4 + i + 3;
            indices.push_back(botlow2);
            indices.push_back(botlow1);
            indices.push_back(bothigh1);
            indices.push_back(botlow2);
            indices.push_back(bothigh1);
            indices.push_back(bothigh2);
        }

        // Create Left face
        u32 count = (u32)positions.size();
        f32 l = R*Math::Radians(angle);
        f32 x = Math::Length(positions[0] - positions[(u32)positions.size()/4]);
        positions.push_back(positions[0]);
        normals.push_back(-Math::Cross(normals[0], Math::Vec3(0.0f, -1.0f, 0.0f)));
        uvs.push_back(Math::Vec2(0.0f, 0.0f));
        positions.push_back(positions[1]);
        normals.push_back(-Math::Cross(Math::Vec3(0.0f, 1.0f, 0.0f), normals[1]));
        uvs.push_back(Math::Vec2(0.0f, 1.0f));
        positions.push_back(positions[count/4]);
        normals.push_back(Math::Cross(normals[count/4], Math::Vec3(0.0f, -1.0f, 0.0f)));
        uvs.push_back(Math::Vec2(Math::Min(x/l, 1.0f), 0.0f));
        positions.push_back(positions[count/4 + 1]);
        normals.push_back(Math::Cross(Math::Vec3(0.0f, 1.0f, 0.0f), normals[count/4 + 1]));
        uvs.push_back(Math::Vec2(Math::Min(x/l, 1.0f), 1.0f));

        indices.push_back((u32)positions.size() - 4);
        indices.push_back((u32)positions.size() - 3);
        indices.push_back((u32)positions.size() - 2);
        indices.push_back((u32)positions.size() - 3);
        indices.push_back((u32)positions.size() - 1);
        indices.push_back((u32)positions.size() - 2);

        // Create Right face
        positions.push_back(positions[count/4 - 2]);
        normals.push_back(Math::Cross(normals[count/4 - 2], Math::Vec3(0.0f, -1.0f, 0.0f)));
        uvs.push_back(Math::Vec2(0.0f, 0.0f));
        positions.push_back(positions[count/4 - 1]);
        normals.push_back(Math::Cross(Math::Vec3(0.0f, 1.0f, 0.0f), normals[count/4 - 1]));
        uvs.push_back(Math::Vec2(0.0f, 1.0f));
        positions.push_back(positions[count/2 - 2]);
        normals.push_back(-Math::Cross(normals[count/2 - 2], Math::Vec3(0.0f, -1.0f, 0.0f)));
        uvs.push_back(Math::Vec2(Math::Min(x/l, 1.0f), 0.0f));
        positions.push_back(positions[count/2 - 1]);
        normals.push_back(-Math::Cross(Math::Vec3(0.0f, 1.0f, 0.0f), normals[count/2 - 2]));
        uvs.push_back(Math::Vec2(Math::Min(x/l, 1.0f), 1.0f));

        indices.push_back((u32)positions.size() - 4);
        indices.push_back((u32)positions.size() - 2);
        indices.push_back((u32)positions.size() - 3);
        indices.push_back((u32)positions.size() - 1);
        indices.push_back((u32)positions.size() - 3);
        indices.push_back((u32)positions.size() - 2);
        

        CalculateTangents(positions, normals, uvs, indices, tangents);
        std::vector<f32> vertices;
        for (u64 i = 0; i < positions.size(); i++) {
            vertices.push_back(positions[i].x);
            vertices.push_back(positions[i].y);
            vertices.push_back(positions[i].z);
            vertices.push_back(normals[i].x);
            vertices.push_back(normals[i].y);
            vertices.push_back(normals[i].z);
            vertices.push_back(tangents[i].x);
            vertices.push_back(tangents[i].y);
            vertices.push_back(tangents[i].z);
            vertices.push_back(uvs[i].x);
            vertices.push_back(uvs[i].y);
        }

        std::vector<Submesh> submeshes;
        submeshes.push_back(Submesh(vertices, indices, 
                                    {{Fly::Attribute::Vec3},
                                     {Fly::Attribute::Vec3},
                                     {Fly::Attribute::Vec3},
                                     {Fly::Attribute::Vec2}}));

        Mesh* mesh = new Mesh("CylinderBrick", submeshes);
        return mesh;
    }

    Mesh* Mesh::CreateSphere(f32 radius, i32 segments, i32 rings) {
        segments = Math::Max(segments, 3);
        rings = Math::Max(rings, 3);

        f32 deltaTheta = Math::PI / rings;
        f32 deltaPhi = 2.0f*Math::PI / segments;

        std::vector<Math::Vec3> positions;
        std::vector<Math::Vec3> normals;
        std::vector<Math::Vec3> tangents;
        std::vector<Math::Vec2> uvs;        

        // North pole
        positions.push_back({0.0f, radius, 0.0f});
        normals.push_back(Math::Vec3(0.0f, 1.0f, 0.0f));
        uvs.push_back({0.0f, 1.0f});
        tangents.push_back(Math::Vec3(0.0f, 0.0f, 1.0f));

        f32 ringSpacing = 1.0f/rings;
        f32 segmentSpacing = 1.0f/segments;

        for (i32 i = 0; i < rings - 1; i++) {
            for (i32 j = 0; j <= segments; j++) {
                Math::Vec2 uv = Math::Vec2(j*segmentSpacing, 1.0f - (i + 1)*ringSpacing);
                f32 phi = uv.x*2.0f*Math::PI;
                f32 theta = (uv.y - 0.5f)*Math::PI;

                f32 c = Math::Cos(theta);

                Math::Vec3 pos = Math::Vec3(c*Math::Sin(phi), Math::Sin(theta), c*Math::Cos(phi))*radius;
                positions.push_back(pos);
                normals.push_back(Math::Normalize(pos));
                uvs.push_back(uv);
                tangents.push_back(Math::Normalize(Math::Vec3(-pos.z, 0.0f, pos.x)));
            }
        }

        // South pole
        positions.push_back({0.0f, -radius, 0.0f});
        normals.push_back(Math::Vec3(0.0f, -1.0f, 0.0f));
        uvs.push_back({0.0f, 0.0f});
        tangents.push_back(Math::Vec3(0.0f, 0.0f, 1.0f));

        std::vector<u32> indices;
        // connecting north pole
        for (i32 i = 1; i <= segments; i++) {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);
        }

        for (i32 i = 0; i < rings - 2; i++) {
            i32 currentRingStart = 1 + (segments+1)*i;
            i32 nextRingStart = 1 + (segments+1)*(i + 1);
            for (i32 j = 0; j <= segments; j++) {
                indices.push_back(nextRingStart + j);
                indices.push_back(nextRingStart + j + 1);
                indices.push_back(currentRingStart + j);
                

                indices.push_back(currentRingStart + j);
                indices.push_back(nextRingStart + j + 1);
                indices.push_back(currentRingStart + j + 1);
            }
        }

        // connecting south pole
        u32 southPole = (u32)(positions.size() - 1);
        for (i32 i = 1; i <= segments; i++) {
            indices.push_back(southPole);
            indices.push_back(southPole - i);
            indices.push_back(southPole - i - 1);
        }

        // For uv-sphere we have to calculate tangents by hand!!!
        // CalculateTangents(positions, normals, uvs, indices, tangents);

        std::vector<f32> vertices;
        for (u64 i = 0; i < positions.size(); i++) {
            vertices.push_back(positions[i].x);
            vertices.push_back(positions[i].y);
            vertices.push_back(positions[i].z);
            vertices.push_back(normals[i].x);
            vertices.push_back(normals[i].y);
            vertices.push_back(normals[i].z);
            vertices.push_back(tangents[i].x);
            vertices.push_back(tangents[i].y);
            vertices.push_back(tangents[i].z);
            vertices.push_back(uvs[i].x);
            vertices.push_back(uvs[i].y);
        }

        std::vector<Submesh> submeshes;
        submeshes.push_back(Submesh(vertices, indices, 
                                    {{Fly::Attribute::Vec3}, 
                                     {Fly::Attribute::Vec3},
                                     {Fly::Attribute::Vec3}, 
                                     {Fly::Attribute::Vec2}}));

        Mesh* mesh = new Mesh("Sphere", submeshes);
        return mesh;
    }
}
