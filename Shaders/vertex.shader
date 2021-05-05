#version 330 core

layout (location = 0) in vec3 position;

out vec4 vertexColor;

uniform mat4 model;

void main()
{
   gl_Position = model * vec4( position, 1.0f );
   vertexColor = vec4(clamp(gl_Position, 0.0f, 1.0f));
}