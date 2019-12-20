#version 460 core

in vec2 UV;

layout(location=0)out vec3 fragColor;

uniform sampler2D screenTexture;

void main()
{
	fragColor = texture(screenTexture, UV).rgb;
}