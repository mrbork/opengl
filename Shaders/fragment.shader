#version 330 core

in vec4 vertexColor;
in vec2 texelCoord;
in vec3 Normal;

out vec4 gl_FragColor;

struct DirectionalLight
{
	vec3 color;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

uniform sampler2D textureSampler;
uniform DirectionalLight directionalLight;

void main()
{
	vec4 ambientColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambientIntensity;
	
	float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
	vec4 diffuseColor = vec4(directionalLight.color, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;
	
	gl_FragColor = texture(textureSampler, texelCoord) * (ambientColor + diffuseColor);
}