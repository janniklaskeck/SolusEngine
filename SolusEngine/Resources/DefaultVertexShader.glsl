#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 vertexUV;

out vec2 UV;

uniform mat4 mvpMatrix;

void main()
{
	gl_Position = mvpMatrix * vec4(position, 1);
	UV = vertexUV;
}