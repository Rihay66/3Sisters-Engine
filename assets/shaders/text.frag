#version 450 core
in vec2 o_TexCoords;
in vec4 o_color;

out vec4 color;

layout (binding = 0) uniform sampler2D text;

void main(){
    color = vec4(o_color) * vec4(1.0, 1.0, 1.0, texture(text, o_TexCoords).r);
}