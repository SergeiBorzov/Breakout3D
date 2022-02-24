[[VERTEX_SHADER]]
#version 430

layout (location = 0) in vec4 uv;
layout (location = 1) in vec3 position;
layout (location = 2) in vec2 size;
layout (location = 3) in float roundness;
layout (location = 4) in vec4 color;
layout (location = 5) in uint textureSlot;


out struct SpriteData {
    vec4 color;
    vec2 size;
    vec2 uv0;
    vec2 uv1;
    float roundness;
} spriteData;

flat out uint v_textureIndex;

void main() {
    spriteData.color = color;
    spriteData.size = size;
    spriteData.uv0 = uv.xy;
    spriteData.uv1 = uv.zw;
    spriteData.roundness = roundness;
    v_textureIndex = textureSlot;
    gl_Position = vec4(position, 1.0f);
}

[[GEOMETRY_SHADER]]
#version 430 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 u_Projection;

in struct SpriteData {
    vec4 color;
    vec2 size;
    vec2 uv0;
    vec2 uv1;
    float roundness;
} spriteData[];

flat in uint v_textureIndex[];

out struct FragmentData {
    vec4 color;
    vec2 size;
    vec2 uv;
    float roundness;
} fragmentData;

flat out uint textureIndex;

void create_quad(vec2 center) {
    vec2 topLeft = center + vec2(-0.5f, -0.5f)*spriteData[0].size; 
    gl_Position = u_Projection*vec4(topLeft, 0.0f, 1.0f);
    fragmentData.color = spriteData[0].color;
    fragmentData.size = spriteData[0].size;
    fragmentData.uv = vec2(spriteData[0].uv0.x, spriteData[0].uv1.y);
    fragmentData.roundness = spriteData[0].roundness;
    textureIndex = v_textureIndex[0];
    EmitVertex();

    vec2 botLeft = center + vec2(-0.5f, 0.5f)*spriteData[0].size;
    gl_Position = u_Projection*vec4(botLeft, 0.0f, 1.0f);
    fragmentData.color = spriteData[0].color;
    fragmentData.size = spriteData[0].size;
    fragmentData.uv = vec2(spriteData[0].uv0.x, spriteData[0].uv0.y);
    fragmentData.roundness =spriteData[0].roundness;
    textureIndex = v_textureIndex[0];
    EmitVertex();

    vec2 topRight = center + vec2(0.5f, -0.5f)*spriteData[0].size;
    gl_Position = u_Projection*vec4(topRight, 0.0f, 1.0f);
    fragmentData.color = spriteData[0].color;
    fragmentData.size = spriteData[0].size;
    fragmentData.uv = vec2(spriteData[0].uv1.x, spriteData[0].uv1.y);
    fragmentData.roundness =spriteData[0].roundness;
    textureIndex = v_textureIndex[0];
    EmitVertex();

    vec2 botRight = center + vec2(0.5f, 0.5f)*spriteData[0].size;
    gl_Position = u_Projection*vec4(botRight, 0.0f, 1.0f);
    fragmentData.color = spriteData[0].color;
    fragmentData.size = spriteData[0].size;
    fragmentData.uv = vec2(spriteData[0].uv1.x, spriteData[0].uv0.y);
    fragmentData.roundness =spriteData[0].roundness;
    textureIndex = v_textureIndex[0];
    EmitVertex();

    EndPrimitive();
}

void main() {
    create_quad(gl_in[0].gl_Position.xy);
}

[[FRAGMENT_SHADER]]
#version 430

in struct FragmentData {
    vec4 color;
    vec2 size;
    vec2 uv;
    float roundness;
} fragmentData;

flat in uint textureIndex;

uniform sampler2D[16] u_TextureTable;

out vec4 fragColor;

float sdfRoundedBox2D(vec2 p, vec2 b, float r) {
    vec2 q = abs(p) - b;
    return length(max(q, 0.0f)) + min(max(q.x, q.y), 0.0f) - r;
}

void main() {
    float value = texture(u_TextureTable[textureIndex], vec2(fragmentData.uv)).r;
    if (value < 0.3f)
        discard;

    float d = sdfRoundedBox2D(fragmentData.uv*fragmentData.size - 0.5*fragmentData.size, 
                              0.5f*fragmentData.size - fragmentData.roundness, 
                              fragmentData.roundness);
    if (d > 0.0f)
        discard;
    
    fragColor = fragmentData.color*value;
}