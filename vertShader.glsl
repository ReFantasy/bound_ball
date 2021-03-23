#version 410

layout (location=0) in vec3 position;
layout (location=1) in float vid;

out vec4 c;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void)
{
	gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);	

    c = vec4(0.0,0.0,0.0,1.0);
	if(vid == 0.0f)
    {
        c[0] = 1.0f;
    }
    else if(vid == 1.0f)
    {
        c[1] = 1.0f;
    }
    else
    {
        c[2] = 1.0f;
    }
} 
