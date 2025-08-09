#version 320 es
precision highp float;
in vec4 o_lineColor;

out vec4 color;

void main(){
    color = vec4(o_lineColor);
}