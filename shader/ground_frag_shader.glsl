#version 460

out vec4 color;
in vec2 tc;
layout (binding=0) uniform sampler2D s;

void main(void)
{	
    //color = vec4(1.0,0.0,0.0,1.0);
    color = texture(s,tc);
}
