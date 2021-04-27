#version 330 core

layout (location = 0) in vec3 position;

uniform float xTranslation;
uniform float yTranslation;

void main()
{
   gl_Position = vec4( position.x + xTranslation , position.y + yTranslation , position.z, 1.0 );
}