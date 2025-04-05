#version 450 core
layout (location = 0) in vec2 vertex; 
layout (location = 1) in vec4 color;

out vec4 o_lineColor;

uniform mat4 projectionView;

void main(){
    gl_Position = projectionView * vec4(vertex, 0.0, 1.0);
    o_lineColor = color;
}