#version 330

// vertex attributes
in vec3 position;
in vec3 normal;
in vec2 texcoord;

// matrices
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

out vec2 tc;	// texture coordinate

void main()
{
	vec4 wpos = model_matrix * vec4(position,1);
	vec4 epos = view_matrix * wpos;
	gl_Position = projection_matrix * epos;

	// pass texture coordinate to fragment shader
	tc = texcoord;
}