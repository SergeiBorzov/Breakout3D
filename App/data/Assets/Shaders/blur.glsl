[[]][[VERTEX_SHADER]]
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
uniform bool u_IsHorizontal;
out vec4 fragColor;

void main() {
    // Gaussian blur: sigma = 2.0, kernel_size = 9
	float kernel[5] = float[](0.20236, 0.179044, 0.124009, 0.067234, 0.028532);
						
	vec2 offset = 1.0f/textureSize(u_ColorAttachment, 0);
    vec3 result = texture(u_ColorAttachment, uv).rgb*kernel[0];

	if (u_IsHorizontal) {
		for(int i = 1; i < 5; ++i)
        {
            result += texture(u_ColorAttachment, uv + vec2(offset.x*i, 0.0f)).rgb*kernel[i];
            result += texture(u_ColorAttachment, uv - vec2(offset.x*i, 0.0f)).rgb*kernel[i];
        }
	}
	else {
		for(int i = 1; i < 5; ++i)
        {
            result += texture(u_ColorAttachment, uv + vec2(0.0f, offset.y*i)).rgb*kernel[i];
            result += texture(u_ColorAttachment, uv - vec2(0.0f, offset.y*i)).rgb*kernel[i];
        }
	}
    
	fragColor = vec4(clamp(result, 0.0f, 1.0f), 1.0f);
}