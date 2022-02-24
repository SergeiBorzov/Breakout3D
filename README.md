
Textures for materials are taken from https://freepbr.com/

Used libraries:
------------------
    * OpenGL 4.3
    * stb_image - for texture loading https://github.com/nothings/stb
    * stb_truetype - font loading and bitmap baking https://github.com/nothings/stb
    * tiny_obj_loader - for .obj loading https://github.com/tinyobjloader/tinyobjloader

List of features:
------------------
    * Generation of tangents via normals and uvs of a mesh
    * Cook-Torrance shading model
    * Forward renderer - scene is rendered in multiple passes with additive blending (8 lights max per pass)
    * Normal mapping
    * Shadow mapping (for directional and spot lights) + Percentage Close Filtering
    * Transparent objects
    * MSAA
    * Anisotropic filtering of textures
    * Batched rendering of text and gui elements
    * Component-based(Unity-like) gameobject system
    * "Native" scripting system
    * Quaternions
    * Post-effects: Gaussian blur, Uncharted 2 tone mapping