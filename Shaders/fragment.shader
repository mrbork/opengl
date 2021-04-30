#version 330 core

layout (location = 0) out vec4 gl_FragColor;

uniform vec3 colorrgb;

void main()
{
   gl_FragColor = vec4( colorrgb[0], colorrgb[ 1 ] , colorrgb[ 2 ] , 1.0f);
}