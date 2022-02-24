#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "../third_party/stb_image.h"

#define STB_TRUETYPE_IMPLEMENTATION 
#include "../third_party/stb_truetype.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "../third_party/tiny_obj_loader.h"


#include "../core/util.h"
#include "../core/error.h"
#include "../core/log.h"
#include "../core/fly_gl.h"
#include "../math/general.h"
#include "../renderer/mesh.h"
#include "../renderer/font.h"
#include "../renderer/material.h"
#include "loader.h"

namespace Fly {
    void Loader::LoadResource(const char* path, Mesh** mesh) {
        std::string warn;
        std::string err;

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;


        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path, false);
        
        if (!warn.empty()) {
            FLY_WARN("%s", warn.c_str());
        }

        if (!err.empty()) {
            FLY_WARN("%s", err.c_str());
        }

        if (!ret) {
            *mesh = nullptr;
            return;
        }

        // Check that the mesh is a triangle mesh
        for (const auto& shape: shapes) {
            for (const auto& fv: shape.mesh.num_face_vertices) {
                if (fv != 3) {
                    mesh = nullptr;
                    FLY_WARN("Loading error, mesh %s is not a triangle mesh", path);
                    return;
                }
            }
        }

        std::vector<Submesh> submeshes;

        struct Vertex {
            f32 x;
            f32 y;
            f32 z;
            f32 nx;
            f32 ny;
            f32 nz;
            f32 u;
            f32 v;
        };

        bool has_uvs = attrib.texcoords.size() > 0;
        bool has_normals = attrib.texcoords.size() >0;

        if (!has_uvs || !has_normals)
            FLY_ERROR("Mesh doesn't contain uvs or normals!"); // Don't load mesh without uvs and normals!
        
        for (const auto& shape: shapes) {
            size_t indexOffset = 0;

            std::vector<Vertex> uniqueVertices;
            std::vector<f32> vertices;
            std::vector<Math::Vec3> positions;
            std::vector<Math::Vec3> normals;
            std::vector<Math::Vec2> uvs;
            std::vector<Math::Vec3> tangents;
            std::vector<u32> indices;

            for (const auto& index: shape.mesh.indices) {
                f32 vx = attrib.vertices[3*index.vertex_index + 0];
                f32 vy = attrib.vertices[3*index.vertex_index + 1];
                f32 vz = attrib.vertices[3*index.vertex_index + 2];

                f32 tx = attrib.texcoords[2*index.texcoord_index + 0];
                f32 ty = attrib.texcoords[2*index.texcoord_index + 1];

                f32 nx = attrib.normals[3*index.normal_index + 0];
                f32 ny = attrib.normals[3*index.normal_index + 1];
                f32 nz = attrib.normals[3*index.normal_index + 2];

                i32 index = -1;
                for (int i = 0; i < uniqueVertices.size(); i++) {
                    if (Math::Abs(vx - uniqueVertices[i].x) < 1e-5f && 
                        Math::Abs(vy - uniqueVertices[i].y) < 1e-5f &&
                        Math::Abs(vz - uniqueVertices[i].z) < 1e-5f &&
                        Math::Abs(nx - uniqueVertices[i].nx) < 1e-5f && 
                        Math::Abs(ny - uniqueVertices[i].ny) < 1e-5f &&
                        Math::Abs(nz - uniqueVertices[i].nz) < 1e-5f &&
                        Math::Abs(tx - uniqueVertices[i].u) < 1e-5f &&
                        Math::Abs(ty - uniqueVertices[i].v) < 1e-5f) {
                        index = i;
                        break;   
                    }
                }
                    

                if (index == -1) {
                    positions.push_back({vx, vy, vz});
                    normals.push_back({nx, ny, nz});
                    uvs.push_back({tx, ty});
                    uniqueVertices.push_back({vx, vy, vz, tx, ty});
                    indices.push_back((u32)uniqueVertices.size() - 1);
                }
                else {
                    indices.push_back((u32)index);
                }
            }

            Mesh::CalculateTangents(positions, normals, uvs, indices, tangents);

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

            submeshes.push_back(Submesh(vertices, indices, 
                                    {{Fly::Attribute::Vec3}, 
                                     {Fly::Attribute::Vec3},
                                     {Fly::Attribute::Vec3}, 
                                     {Fly::Attribute::Vec2}}));
        }

        *mesh = new Mesh(path, submeshes);
    }

    void Loader::LoadResource(const char* path, Font** font, f32 fontSize) {
        u8* buffer = (u8*)Util::ReadFileToString(path);
        if (!buffer) {
            FLY_WARN("Failed to load font from file %s", path);
            *font = nullptr;
            return;
        }

        u32 bitmapWidth = 1024;
        u32 bitmapHeight = 1024;

        u8* bitmapBuffer = (u8*)malloc(sizeof(u8)*bitmapWidth*bitmapHeight);

        stbtt_fontinfo* fontInfo = (stbtt_fontinfo*)malloc(sizeof(stbtt_fontinfo)); 


        if (!stbtt_InitFont(fontInfo, buffer, stbtt_GetFontOffsetForIndex(buffer, 0))) {
            FLY_WARN("Error reading truetype font!");
            free(buffer);
            free(bitmapBuffer);
            *font = nullptr;
            return;
        }

        *font = new Font();
        (*font)->m_Size = fontSize;
        (*font)->m_BakedChars = malloc(sizeof(stbtt_bakedchar)*96); // ASCII 32..126 is 95 glyphs;
        (*font)->m_FontInfo = fontInfo;
        (*font)->m_FontBuffer = buffer;

        // TODO: check on errors !!!
        stbtt_BakeFontBitmap(buffer, 0, fontSize, bitmapBuffer, bitmapWidth, bitmapHeight, 32, 96, (stbtt_bakedchar*)(*font)->m_BakedChars); // no guarantee this fits!

        (*font)->m_Bitmap = new Texture();
        (*font)->m_Bitmap->m_Width = bitmapWidth;
        (*font)->m_Bitmap->m_Height = bitmapHeight;

        GLCALL(glGenTextures(1, &((*font)->m_Bitmap->m_Handle)));
        GLCALL(glBindTexture(GL_TEXTURE_2D, (*font)->m_Bitmap->m_Handle));
        GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, bitmapWidth, bitmapHeight, 0, GL_RED, GL_UNSIGNED_BYTE, bitmapBuffer));

        (*font)->m_Bitmap->SetWrapMode(Texture::Wrap::Repeat);
        (*font)->m_Bitmap->SetFilteringMode(Texture::Filtering::Linear);
        free(bitmapBuffer);

        (**font).m_Name = std::to_string((int)fontSize);
    }

    void Loader::LoadResource(const char* path, Shader** shader) {
        bool has_vertex = false;
        bool has_fragment = false;
        bool has_geometry = false;

        char* string = Util::ReadFileToString(path);
        if (!string) {
            FLY_WARN("Failed to load shader from file %s", path);
            *shader = nullptr;
            return;
        }
        
        std::string shader_str(string);
        free(string);

        std::string vertex_keyword = "[[VERTEX_SHADER]]";
        std::string fragment_keyword = "[[FRAGMENT_SHADER]]";
        std::string geometry_keyword = "[[GEOMETRY_SHADER]]";

        std::size_t v_index = shader_str.find(vertex_keyword);
        has_vertex = v_index != std::string::npos;

        std::size_t f_index = shader_str.find(fragment_keyword);
        has_fragment = f_index != std::string::npos;

        std::size_t g_index = shader_str.find(geometry_keyword);
        has_geometry = g_index != std::string::npos;

        if (!has_vertex) {
            FLY_WARN("Couldn't create shader, vertex shader wasn't specified.");
            *shader = nullptr;
            return;
        }

        if (!has_fragment) {
            FLY_WARN("Couldn't create shader, fragment shader wasn't specified.");
            *shader = nullptr;
            return;
        }

        if (has_geometry && !((v_index < g_index) && (g_index < f_index)) ||
            !has_geometry && !(v_index < f_index)) {
            FLY_WARN("Couldn't create shader, shader order is wrong.");
            *shader = nullptr;
            return;
        }

        // Cutting shader string into sources
        std::string vertex_source, geometry_source, fragment_source;
        if (has_geometry) {
            vertex_source = shader_str.substr(v_index + vertex_keyword.length(),
                                              g_index - v_index - vertex_keyword.length());
            geometry_source = shader_str.substr(g_index + geometry_keyword.length(),
                                                f_index - g_index - geometry_keyword.length());
        }
        else {
            vertex_source = shader_str.substr(v_index + vertex_keyword.length(),
                                              f_index - v_index - vertex_keyword.length());
        }
        fragment_source = shader_str.substr(f_index + fragment_keyword.length());


        // Compiling shaders
        u32 vertex_handle = 0;
        GLCALL(vertex_handle = glCreateShader(GL_VERTEX_SHADER));
        const char* vertex_cstr = vertex_source.c_str();
        GLCALL(glShaderSource(vertex_handle, 1, &vertex_cstr, NULL));
        GLCALL(glCompileShader(vertex_handle));

        u32 fragment_handle = 0;
        GLCALL(fragment_handle = glCreateShader(GL_FRAGMENT_SHADER));
        const char* fragment_cstr = fragment_source.c_str();
        GLCALL(glShaderSource(fragment_handle, 1, &fragment_cstr, NULL));
        GLCALL(glCompileShader(fragment_handle));

        u32 geometry_handle = 0;
        if (has_geometry) {
            GLCALL(geometry_handle = glCreateShader(GL_GEOMETRY_SHADER));
            const char* geometry_cstr = geometry_source.c_str();
            GLCALL(glShaderSource(geometry_handle, 1, &geometry_cstr, NULL));
            GLCALL(glCompileShader(geometry_handle));
        }
        
        // Checking for compilation errors
        bool compilation_failed = false;
        i32 success;
        char info_buf[512];

        GLCALL(glGetShaderiv(vertex_handle, GL_COMPILE_STATUS, &success));
        if (!success) {
            GLCALL(glGetShaderInfoLog(vertex_handle, 512, NULL, info_buf));            
            FLY_WARN("Vertex shader compilation error file %s: %s", path, info_buf);
            compilation_failed = true;
        }

        if (has_geometry) {
            GLCALL(glGetShaderiv(geometry_handle, GL_COMPILE_STATUS, &success));
            if (!success) {
                GLCALL(glGetShaderInfoLog(geometry_handle, 512, NULL, info_buf));
                FLY_WARN("Geometry shader compilation error %s: %s", path, info_buf);
                compilation_failed = true;
            }
        }

        GLCALL(glGetShaderiv(fragment_handle, GL_COMPILE_STATUS, &success));
        if (!success) {
            GLCALL(glGetShaderInfoLog(fragment_handle, 512, NULL, info_buf));
            FLY_WARN("Fragment shader compilation error %s: %s", path, info_buf);
            compilation_failed = true;
        }

        if (compilation_failed) {
            *shader = nullptr;
        }

        *shader = new Shader();
        // Linking shaders
        GLCALL((**shader).m_Handle = glCreateProgram());
        GLCALL(glAttachShader((**shader).m_Handle, vertex_handle));
        GLCALL(glAttachShader((**shader).m_Handle, fragment_handle));
        if (has_geometry) {
            GLCALL(glAttachShader((**shader).m_Handle, geometry_handle));
        }
        GLCALL(glLinkProgram((**shader).m_Handle));

        // Checking for linking errors
        GLCALL(glGetProgramiv((**shader).m_Handle, GL_LINK_STATUS, &success));
        if (!success) {
            GLCALL(glGetProgramInfoLog((**shader).m_Handle, 512, NULL, info_buf));
            FLY_WARN("Shader linking failed %s: %s", path, info_buf);
            delete *shader;
            *shader = nullptr;
        }

        GLCALL(glDetachShader((**shader).m_Handle, vertex_handle));
        GLCALL(glDeleteShader(vertex_handle));
        GLCALL(glDetachShader((**shader).m_Handle, fragment_handle));
        GLCALL(glDeleteShader(fragment_handle));
        if (has_geometry) {
            GLCALL(glDetachShader((**shader).m_Handle, geometry_handle));
            GLCALL(glDeleteShader(geometry_handle));
        }

        (**shader).m_Name = path;        
    }
    
    void Loader::LoadResource(const char* path, Texture** texture) {
        i32 width; i32 height; i32 channels;
        stbi_set_flip_vertically_on_load(true);

        unsigned char* image = stbi_load(path, &width, &height, &channels, 0);

        if (!image) {
            FLY_WARN("Failed to load texture %s", path);
            return;
        }
        
        *texture = new Texture();
        (*texture)->m_Name = path;
        (*texture)->m_Width = (u32)width;
        (*texture)->m_Height = (u32)height;
        GLCALL(glGenTextures(1, &(**texture).m_Handle));
        GLCALL(glBindTexture(GL_TEXTURE_2D, (**texture).m_Handle));

        (*texture)->SetWrapMode(Texture::Wrap::Repeat);
        (*texture)->GenerateMipmaps();
        (*texture)->SetFilteringMode(Texture::Filtering::Linear);

        switch (channels) {            
            case 1: {
                GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED,
                                    GL_UNSIGNED_BYTE, image));
                break;   
            }
            case 2: {
                GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG,
                                    GL_UNSIGNED_BYTE, image));
                break;
            }
            case 3: {
                GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                                    GL_UNSIGNED_BYTE, image));
                break;
            }
            case 4: {
                GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                                    GL_UNSIGNED_BYTE, image));
                break;
            }
        }
        
        stbi_image_free(image);
        return;
    }

    void Loader::LoadResource(const char* path, Material** material) {
        *material = nullptr;
    }
    
}
