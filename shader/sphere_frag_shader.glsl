#version 460

in vec2 tc;
out vec4 color;

layout (binding=0) uniform sampler2D s;
void main(void)
{	
    //color = texture(s,tc);
    color = vec4(1.0f,0.4f,0.0f,1.0);
}
