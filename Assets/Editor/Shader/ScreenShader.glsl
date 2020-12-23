#ifdef VERTEX_SHADER

layout (location = 0) in vec2 position;

out vec2 UV;

void main()
{
	UV = (position.xy + vec2(1,1)) / 2.0;
	gl_Position = vec4(position, 0.0, 1.0);
}
#endif

#ifdef FRAGMENT_SHADER
in vec2 UV;

layout(location=0)out vec3 fragColor;

uniform sampler2D screenTexture;

void main()
{
	fragColor = texture(screenTexture, UV).rgb;
}
#endif
