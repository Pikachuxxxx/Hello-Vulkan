#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inColor;

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
    mat4 _padding1;
    mat4 _padding2;
} ubo;

layout(location = 0) out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Color;
} vs_out;

layout (push_constant) uniform ModelPushConstantData{
    mat4 model;
}modelPushConstantData;

void main() {
    gl_PointSize    = 5.0;
    mat4 rotView = mat4(mat3(ubo.view)); // remove translation from the view matrix
    vec4 clipPos = ubo.proj * rotView * vec4(inPosition, 1.0);

    gl_Position = clipPos.xyww;

    vs_out.FragPos      = inPosition;
    vs_out.Normal       = inNormal;
    vs_out.TexCoords    = inTexCoord;
    vs_out.Color        = inColor;
}
