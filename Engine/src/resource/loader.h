#ifndef FLY_LOADER_H
#define FLY_LOADER_H

namespace Fly {
    class Mesh;
    class Shader;
    class Texture;
    class Font;
    class Material;

    class Loader {
        friend class ResourceManager;
    private:
        static void LoadResource(const char* path, Mesh** mesh);
        static void LoadResource(const char* path, Shader** shader);
        static void LoadResource(const char* path, Texture** texture);
        static void LoadResource(const char* path, Font** font, f32 fontSize);
        static void LoadResource(const char* path, Material** material);
    };
}

#endif /* FLY_LOADER_H */
