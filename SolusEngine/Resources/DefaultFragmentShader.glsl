#version 460 core

in vec2 UV;

out vec4 FragColor;

uniform sampler2D textureSampler;

void main()
{
   FragColor = vec4(texture(textureSampler, UV).rgb, 1);
}