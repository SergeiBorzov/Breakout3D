[[VERTEX_SHADER]]
#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec2 uv;

uniform mat4 u_Model;

layout(std140, binding = 0) uniform Camera {
    mat4 projection;
    mat4 view;
};

out struct FragmentData {
    mat3 tbn;
    vec4 position;
    vec2 uv;
} fragmentData;

void main() {
    mat4 inverseTranspose = transpose(inverse(u_Model));
    vec3 normalWS = normalize(vec3(inverseTranspose*vec4(normal, 0.0f)));
    vec3 tangentWS = normalize(vec3(u_Model*vec4(tangent, 0.0f)));
    tangentWS = normalize(tangentWS - dot(tangentWS, normalWS)*normalWS);
    vec3 bitangentWS = normalize(cross(normalWS, tangentWS));

    fragmentData.tbn = transpose(mat3(tangentWS, bitangentWS, normalWS));
    vec4 positionWS = u_Model*vec4(position, 1.0f);
    fragmentData.position = positionWS;
    fragmentData.uv = uv;

    gl_Position = projection*view*positionWS;
}

[[FRAGMENT_SHADER]]
#version 430

#define PI 3.1415926538f

// using vec4 because some implementations for std140 are incorrect (https://www.khronos.org/opengl/wiki/Interface_Block_(GLSL))
struct Light {
    mat4 lightMat;  // light Matrix
    vec4 position;  // position WS,
    vec4 direction; // direction WS
    vec4 intensity;
    float outerAngleCos;
    float innerAngleCos;
    int type; // 0 - point, 1 - spot
    int generateShadows;
};

layout(std140, binding = 1) uniform Lights {
    Light lights[8];
    int numLights;
};

in struct FragmentData {
    mat3 tbn;
    vec4 position;
    vec2 uv;
} fragmentData;


out vec4 fragColor;

uniform vec3 u_Albedo;
uniform vec2 u_Tiling;
uniform vec3 u_CameraPos;
uniform float u_Threshold;
uniform float u_Opaqueness;

layout (binding = 0) uniform sampler2D u_AlbedoMap;
layout (binding = 1) uniform sampler2D u_NormalMap;
layout (binding = 2) uniform sampler2D u_RoughnessMap;
layout (binding = 3) uniform sampler2D u_MetallicMap;
layout (binding = 4) uniform sampler2D u_NoiseMap;

uniform sampler2D u_ShadowMaps[8];

float random(vec2 p) {
	return fract(sin(dot(p, vec2(12.9898f, 78.233f)))*43758.5453f);
}

float pcfAverage(int i, vec3 projCoords, int kernelRadius, float bias) {
    float pcfShadow = 0.0f;
    vec2 texelSize = 1.0f/textureSize(u_ShadowMaps[i], 0);
    float currentDepth = projCoords.z;
    for(int x = -kernelRadius; x <= kernelRadius; x++) {
        for(int y = -kernelRadius; y <= kernelRadius; ++y) {
            float pcfDepth = texture(u_ShadowMaps[i], projCoords.xy + vec2(x, y) * texelSize).r; 
            pcfShadow += currentDepth - bias < pcfDepth ? 1.0f : 0.00f;        
        }    
    }
    pcfShadow /= 25.0f;

    return pcfShadow;
}

vec3 fresnelSchlick(vec3 halfTS, vec3 viewTS, vec3 f0) {
    return f0 + (1.0f - f0)*pow(max(1.0 - max(dot(halfTS, viewTS), 0.0f), 0.0f), 5.0f);
}

float DistributionGGX(vec3 normalTS, vec3 halfTS, float roughness) {
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(normalTS, halfTS), 0.0f);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2*(a2 - 1.0f) + 1.0f);
    denom = PI*denom*denom;
    return num/denom;
}

float GeometrySchlick(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 normalTS, vec3 viewTS, vec3 lightTS, float roughness) {
    float NdotV = max(dot(normalTS, viewTS), 0.0);
    float NdotL = max(dot(normalTS, lightTS), 0.0);
    float ggx2 = GeometrySchlick(NdotV, roughness);
    float ggx1 = GeometrySchlick(NdotL, roughness);
	
    return ggx1*ggx2;
}

vec3 CookTorrance(vec3 normalTS, vec3 viewTS, vec3 lightTS, vec3 halfTS, vec3 f0, float roughness, out vec3 kS) {
    // Cook-Torrance
    float D = DistributionGGX(normalTS, halfTS, roughness);        
    float G = GeometrySmith(normalTS, viewTS, lightTS, roughness);      
    vec3 F = fresnelSchlick(halfTS, viewTS, f0);

    kS = F;	  
        
    vec3 numerator = D*G*F;
    float denominator = 4.0f*max(dot(normalTS, viewTS), 0.0f)*max(dot(normalTS, lightTS), 0.0f);
    return numerator/max(denominator, 0.001f); //prevent division by 0
}

vec3 processDirectionalLight(vec3 normalTS, vec3 viewTS, vec3 albedo, vec3 f0, float roughness, float metallic, int i) {
    vec3 lightTS = fragmentData.tbn*lights[i].direction.xyz;
    vec3 halfTS = normalize(lightTS + viewTS);
    vec3 radiance = lights[i].intensity.xyz;

    float shadow = float(lightTS.z > 0.0f);
    if (lights[i].generateShadows == 1) {
        vec4 positionLS = lights[i].lightMat*fragmentData.position;
        vec3 projCoords = positionLS.xyz/positionLS.w;
        projCoords = projCoords*0.5f + 0.5f;

        float bias = max(0.05f*(1.0f - lightTS.z), 0.005f);  
        float pcfShadow = pcfAverage(i, projCoords, 2, bias);
        shadow *= pcfShadow;
    }

    vec3 kS = vec3(0.0f);
    vec3 specular = CookTorrance(normalTS, viewTS, lightTS, halfTS, f0, roughness, kS);

    vec3 kD = 1.0f - kS;
    kD *= 1.0f - metallic; // no diffuse for metalls

    return (kD*albedo/PI + specular)*radiance*max(dot(lightTS, normalTS), 0.0f)*shadow;
}

vec3 processSpotLight(vec3 normalTS, vec3 viewTS, vec3 albedo, vec3 f0, float roughness, float metallic, int i) {
    vec3 dir = lights[i].position.xyz - fragmentData.position.xyz;
    vec3 lightTS = fragmentData.tbn*normalize(dir);

    vec3 coneAxis = fragmentData.tbn*lights[i].direction.xyz;
    float cos_theta = max(dot(coneAxis, lightTS), 0.0f);
    // if outside of the outer cone
    if (cos_theta < lights[i].outerAngleCos) {
        return vec3(0.0f, 0.0f, 0.0f);
    }

    vec3 halfTS = normalize(lightTS + viewTS);
    float shadow = float(lightTS.z > 0.0f);
    vec3 radiance = lights[i].intensity.xyz;

    
    if (lights[i].generateShadows == 1) {
        vec4 positionLS = lights[i].lightMat*fragmentData.position;
        vec3 projCoords = positionLS.xyz/positionLS.w;
        projCoords = projCoords*0.5f + 0.5f;

        float bias = max(0.05f*(1.0f - lightTS.z), 0.005f);  
        float pcfShadow = pcfAverage(i, projCoords, 2, bias);
        shadow *= pcfShadow;
    }

    vec3 kS = vec3(0.0f);
    vec3 specular = CookTorrance(normalTS, viewTS, lightTS, halfTS, f0, roughness, kS);

    vec3 kD = 1.0f - kS;
    kD *= 1.0f - metallic; // no diffuse for metalls

    vec3 color = (kD*albedo/PI + specular)*radiance*max(dot(lightTS, normalTS), 0.0f)*shadow;
    // inside inner cone
    if (cos_theta >= lights[i].innerAngleCos) {
        return color/dot(dir, dir); // apply attenuation
    }

    // between inside and outside cone - quadratic attenuation
    float delta = (cos_theta - lights[i].outerAngleCos)/(lights[i].innerAngleCos - lights[i].outerAngleCos);
    return (color*delta*delta*delta*delta)/dot(dir, dir); // apply attenuation
}

vec3 processPointLight(vec3 normalTS, vec3 viewTS, vec3 albedo, vec3 f0, float roughness, float metallic, int i) {
    vec3 dir = lights[i].position.xyz - fragmentData.position.xyz;
    vec3 lightTS = fragmentData.tbn*normalize(dir);
    vec3 halfTS = normalize(lightTS + viewTS);
    vec3 radiance = lights[i].intensity.xyz/dot(dir, dir);
    float shadow = float(lightTS.z > 0.0f);

    vec3 kS = vec3(0.0f);
    vec3 specular = CookTorrance(normalTS, viewTS, lightTS, halfTS, f0, roughness, kS);

    vec3 kD = 1.0f - kS;
    kD *= 1.0f - metallic; // no diffuse for metalls

    return (kD*albedo/PI + specular)*radiance*max(dot(lightTS, normalTS), 0.0f)*shadow;    
}

void main() {
    vec2 noiseTiling = vec2(3.0, 3.0f);
    float noiseValue = texture(u_NoiseMap, fragmentData.uv*u_Tiling*noiseTiling).r;
    if (noiseValue < u_Threshold) {
        discard;
    }

    vec3 albedo = pow(texture(u_AlbedoMap, fragmentData.uv*u_Tiling).rgb, vec3(2.2f));
    //vec3 albedo = texture(u_AlbedoMap, fragmentData.uv*u_Tiling).rgb;
    albedo = u_Albedo*albedo;
    float roughness = texture(u_RoughnessMap, fragmentData.uv*u_Tiling).r;
    float metallic = 1.0f - texture(u_MetallicMap, fragmentData.uv*u_Tiling).r;
    
    
    vec3 normalTS = texture(u_NormalMap, fragmentData.uv*u_Tiling).rgb;
    normalTS = normalize(normalTS*2.0 - 1.0);
    vec3 viewTS = normalize(fragmentData.tbn*normalize(u_CameraPos - fragmentData.position.xyz));

    //vec3 color = albedo*vec3(0.04f); // just some ambient light
    vec3 color = vec3(0.0f);

    vec3 f0 = vec3(0.04); 
    f0 = mix(f0, albedo, metallic);
    // all types of lights are combined in one shader just for simplicity
    for (int i = 0; i < numLights; i++) {
        if (lights[i].type == 0) { // directional light case
            color += processDirectionalLight(normalTS, viewTS, albedo, f0, roughness, metallic, i);
        }
        else if (lights[i].type == 1) { // point light case
            color += processPointLight(normalTS, viewTS, albedo, f0, roughness, metallic, i);
        }
        else { // spot light case
            color += processSpotLight(normalTS, viewTS, albedo, f0, roughness, metallic, i);
        }
    }

    vec3 border = vec3(0.2f, 0.6f, 1.0f)*3.0f;
    if (noiseValue - 0.12f > 0.0f && noiseValue - 0.12f < u_Threshold) {
        float t = (noiseValue - 0.12f)/u_Threshold;
        color = mix(border, color, clamp(t*t, 0.0f, 1.0f));
        //color = length(color) < 0.01f ? vec3(0.0f) : border; // quick fix for turning off the lighting on the brick when the ball is lost
    }
        
    fragColor = vec4(color, u_Opaqueness);
}