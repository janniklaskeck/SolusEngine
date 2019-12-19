#version 460 core

in vec4 colorVS;

layout(location=0)out vec4 fragColor;

void main()
{
	fragColor = colorVS;
}