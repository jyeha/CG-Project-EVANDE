#version 330

// input from vertex shader
in vec2 tc;

// the only output variable
out vec4 fragColor;

// texture sampler
uniform sampler2D TEX;
uniform bool use_texture;
uniform vec4 diffuse;
uniform bool is_transparent;

void main()
{
	if (is_transparent)		fragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	else					fragColor = use_texture ? texture2D( TEX, tc ) : diffuse;
}