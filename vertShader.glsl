#version 410

layout (location=0) in vec3 position;
layout (location=1) in vec2 t;
layout (location=2) in vec3 normal;

out vec4 c;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void)
{
	gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);	

    c = vec4(normal,1.0);

} 
