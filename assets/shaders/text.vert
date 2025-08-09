#version 450 core
layout (location = 0) in vec3 vertex; 
layout (location = 1) in vec2 texCoords; 
layout (location = 2) in vec4 color;

out vec2 o_TexCoords;
out vec4 o_color;

uniform mat4 projectionView;

void main(){
    gl_Position = projectionView * vec4(vertex, 1.0);
    o_TexCoords = texCoords;
    o_color = color;
}