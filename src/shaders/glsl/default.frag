#version 450
#extension GL_ARB_separate_shader_objects : enable
//
layout(location = 0) in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Color;
} vs_in;

layout(location = 0) out vec4 outColor;

//layout(set = 0, binding = 1) uniform sampler2D gridSampler;
//layout(set = 0, binding = 2) uniform sampler2D checkerTex;
//layout(set = 0, binding = 3, rgba32f) uniform image2D storageImage;

void main() {

//    outColor = vec4( 0.8, 0.4, 0.0, 1.0 );
//    outColor = vec4(vs_in.Normal, 1.0f);
//    if(gl_FragCoord.x < 640)
//        outColor = texture(gridSampler, vs_in.TexCoords);
//    else
//        outColor = texture(checkerTex, vs_in.TexCoords);
//
//    float color = imageLoad(storageImage, ivec2(gl_FragCoord)).x;
//    color++;
//    imageStore(storageImage, ivec2(gl_FragCoord), vec4(color, 0.0f, 0.0f, 0.0f));
    outColor = vec4(vs_in.Normal, 1.0f);
}
