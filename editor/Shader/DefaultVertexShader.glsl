#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 vertexUV;

out vec2 UV;
out vec3 wsPosition;
out vec3 vsNormal;
out vec3 vsEyeDirection;
out vec3 vsLightDirection;

uniform mat4 mvpMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec3 wsLightPosition;

void main()
{
	gl_Position = mvpMatrix * vec4(position, 1);
	wsPosition = (modelMatrix * vec4(position, 1.0)).xyz;
	
	vec3 vsPosition = (viewMatrix * modelMatrix * vec4(position, 1.0)).xyz;
	vsEyeDirection = vec3(0, 0, 0) - vsPosition;
	
	vec3 vsLightPosition = (viewMatrix * vec4(wsLightPosition, 1.0)).xyz;
	vsLightDirection = vsLightPosition + vsEyeDirection;

	vsNormal = (viewMatrix * modelMatrix * vec4(normal, 0.0)).xyz;

	UV = vertexUV;
}