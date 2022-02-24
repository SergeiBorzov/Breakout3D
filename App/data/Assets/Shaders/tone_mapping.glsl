[[VERTEX_SHADER]]
#version 430 core
void main() {}

[[GEOMETRY_SHADER]]
#version 430 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out vec2 uv;
void main() {
    gl_Position = vec4(1.0f, 1.0f, 0.5f, 1.0f);
    uv = vec2(1.0f, 1.0f);
    EmitVertex();

    gl_Position = vec4(-1.0f, 1.0f, 0.5f, 1.0f);
    uv = vec2(0.0f, 1.0f); 
    EmitVertex();

    gl_Position = vec4(1.0f, -1.0f, 0.5f, 1.0f);
    uv = vec2(1.0f, 0.0f); 
    EmitVertex();

    gl_Position = vec4(-1.0f, -1.0f, 0.5f, 1.0f);
    uv = vec2(0.0f, 0.0f); 
    EmitVertex();

    EndPrimitive(); 
}

[[FRAGMENT_SHADER]]
#version 430 core

in vec2 uv;
layout (binding = 10) uniform sampler2D u_ColorAttachment;
out vec4 fragColor;

// from https://www.shadertoy.com/view/lslGzl
vec3 Uncharted2ToneMapping(vec3 color) {
	float A = 0.15;
	float B = 0.50;
	float C = 0.10;
	float D = 0.20;
	float E = 0.02;
	float F = 0.30;
	float W = 11.2;
	float exposure = 2.f;
	color *= exposure;
	color = ((color*(A*color + C*B) + D*E) / (color*(A*color + B) + D*F)) - E/F;
	float white = ((W*(A*W + C*B) + D*E)/(W*(A*W + B) + D*F)) - E/F;
	color /= (white + 0.01f);
	color = pow(color, vec3(1.0f/2.2f)); // gamma correction;
	return color;
}

void main() {
    fragColor = vec4(Uncharted2ToneMapping(texture(u_ColorAttachment, uv).rgb), 1.0f);
}