#version 460 core

layout (location = 0) in vec2 position;

out vec4 colorVS;

uniform vec4 color;

void main()
{
	colorVS = color;
	gl_Position = vec4(position, 0.0, 1.0);
}