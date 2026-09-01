$input v_view, v_normal, v_shadowcoord, v_color0

#include <bgfx_shader.sh>
#include "shaderlib.sh"

#define SHADOW_PACKED_DEPTH 0
#include "fs_shadow.sh"