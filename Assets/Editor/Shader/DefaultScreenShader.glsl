#ifdef VERTEX_SHADER
layout (location = 0) in vec2 position;

out vec4 colorVS;

uniform vec4 color;

void main()
{
	colorVS = color;
	gl_Position = vec4(position, 0.0, 1.0);
}
#endif

#ifdef FRAGMENT_SHADER
in vec4 colorVS;

layout(location=0)out vec4 fragColor;

void main()
{
	fragColor = colorVS;
}
#endif
