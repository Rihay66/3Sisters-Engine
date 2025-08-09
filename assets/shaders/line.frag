#version 450 core
in vec4 o_lineColor;

out vec4 color;

void main(){
    color = vec4(o_lineColor);
}