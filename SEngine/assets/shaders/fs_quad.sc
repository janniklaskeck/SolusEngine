$input v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_texColor, 0);

void main()
{
	gl_FragColor = texture2D(s_texColor, vec2(v_texcoord0.x, 1.0 - v_texcoord0.y));
	//gl_FragColor = Vec4(1.0, 0.0, 0.0, 1.0);
}
