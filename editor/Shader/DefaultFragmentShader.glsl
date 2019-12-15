#version 460 core

in vec2 UV;
in vec3 wsPosition;
in vec3 vsNormal;
in vec3 vsEyeDirection;
in vec3 vsLightDirection;

layout(location=0)out vec4 fragColor;

uniform sampler2D textureSampler;
uniform mat4 modelViewMatrix;
uniform vec3 wsLightPosition;

void main()
{
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	float lightPower = 50.0;

	vec3 diffuseColor = texture(textureSampler, UV).rgb;
	vec3 ambientColor = vec3(0.1, 0.1, 0.1) * diffuseColor;
	vec3 specularColor = vec3(0.3, 0.3, 0.3);

	float distance = length(wsLightPosition - wsPosition);

	vec3 normal = normalize(vsNormal);
	vec3 lightDirection = normalize(vsLightDirection);
	float cosTheta = clamp(dot(normal, lightDirection), 0.0, 1.0);

	vec3 eyeVector = normalize(vsEyeDirection);
	vec3 reflection = reflect(-lightDirection, normal);

	float cosAlpha = clamp(dot(eyeVector, reflection), 0.0, 1.0);

	vec3 diffuse = diffuseColor * lightColor * lightPower * cosTheta / (distance * distance);
	vec3 specular = specularColor * lightColor * lightPower * pow(cosAlpha, 5) / (distance * distance);
	vec3 finalColor = ambientColor + diffuse + specular;

	fragColor = vec4(finalColor, 1.0);
}