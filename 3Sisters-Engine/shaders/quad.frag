#version 450 core
in vec2 o_TexCoords;
in vec4 o_quadColor;
in float o_TexIndex;

out vec4 color;

uniform sampler2D image[32];

void main(){   
    vec4 texColor = o_quadColor;

    switch(int(o_TexIndex))
	{
		case  0: texColor *= texture(image[ 0], o_TexCoords); break;
		case  1: texColor *= texture(image[ 1], o_TexCoords); break;
		case  2: texColor *= texture(image[ 2], o_TexCoords); break;
		case  3: texColor *= texture(image[ 3], o_TexCoords); break;
		case  4: texColor *= texture(image[ 4], o_TexCoords); break;
		case  5: texColor *= texture(image[ 5], o_TexCoords); break;
		case  6: texColor *= texture(image[ 6], o_TexCoords); break;
		case  7: texColor *= texture(image[ 7], o_TexCoords); break;
		case  8: texColor *= texture(image[ 8], o_TexCoords); break;
		case  9: texColor *= texture(image[ 9], o_TexCoords); break;
		case 10: texColor *= texture(image[10], o_TexCoords); break;
		case 11: texColor *= texture(image[11], o_TexCoords); break;
		case 12: texColor *= texture(image[12], o_TexCoords); break;
		case 13: texColor *= texture(image[13], o_TexCoords); break;
		case 14: texColor *= texture(image[14], o_TexCoords); break;
		case 15: texColor *= texture(image[15], o_TexCoords); break;
		case 16: texColor *= texture(image[16], o_TexCoords); break;
		case 17: texColor *= texture(image[17], o_TexCoords); break;
		case 18: texColor *= texture(image[18], o_TexCoords); break;
		case 19: texColor *= texture(image[19], o_TexCoords); break;
		case 20: texColor *= texture(image[20], o_TexCoords); break;
		case 21: texColor *= texture(image[21], o_TexCoords); break;
		case 22: texColor *= texture(image[22], o_TexCoords); break;
		case 23: texColor *= texture(image[23], o_TexCoords); break;
		case 24: texColor *= texture(image[24], o_TexCoords); break;
		case 25: texColor *= texture(image[25], o_TexCoords); break;
		case 26: texColor *= texture(image[26], o_TexCoords); break;
		case 27: texColor *= texture(image[27], o_TexCoords); break;
		case 28: texColor *= texture(image[28], o_TexCoords); break;
		case 29: texColor *= texture(image[29], o_TexCoords); break;
		case 30: texColor *= texture(image[30], o_TexCoords); break;
		case 31: texColor *= texture(image[31], o_TexCoords); break;
	}

    color = texColor;
}