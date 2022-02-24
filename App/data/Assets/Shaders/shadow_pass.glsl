[[VERTEX_SHADER]]
#version 430 core

layout (location = 0) in vec3 position;
layout (location = 3) in vec2 uv;

uniform mat4 u_ProjectionView;

uniform mat4 u_Model;

out struct FragmentData {
    vec2 uv;
} fragmentData;

void main() {
    fragmentData.uv = uv;
    gl_Position = u_ProjectionView*u_Model*vec4(position, 1.0f);
}

[[FRAGMENT_SHADER]]
#version 430 core

layout (binding = 4) uniform sampler2D u_NoiseMap;
uniform vec2 u_Tiling;
uniform float u_Threshold;

in struct FragmentData {
    vec2 uv;
} fragmentData;

void main() {
    float noiseValue = texture(u_NoiseMap, fragmentData.uv*u_Tiling*5.0f).r;
    if (noiseValue < u_Threshold) {
        discard;
    }
}