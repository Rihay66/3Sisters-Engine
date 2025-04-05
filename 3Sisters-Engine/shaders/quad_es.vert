#version 320 es
layout (location = 0) in vec2 vertex; 
layout (location = 1) in vec2 texCoords; 
layout (location = 2) in float texIndex;
layout (location = 3) in vec4 color;

out vec2 o_TexCoords;
out float o_TexIndex;
out vec4 o_spriteColor;

uniform mat4 projectionView;

void main(){
    gl_Position = projectionView * vec4(vertex, 0.0, 1.0);
    o_TexCoords = texCoords;
    o_TexIndex = texIndex;
    o_spriteColor = color;
}