#version 330 core

in vec4 vertexColor;
in vec2 texelCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 gl_FragColor;

struct DirectionalLight
{
	vec3 color;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

struct Material
{
	float specularIntensity;
	float Shininess;
};

uniform sampler2D textureSampler;
uniform DirectionalLight directionalLight;
uniform Material material;
uniform vec3 cameraPos;

void main()
{
	vec4 ambientColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambientIntensity;
	
	float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
	vec4 diffuseColor = vec4(directionalLight.color, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;
	
	vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	
	if (diffuseFactor > 0.0f)
	{
		vec3 EyetoFrag = normalize(cameraPos - FragPos);
		vec3 reflectedLight = normalize(reflect(directionalLight.direction, normalize(Normal)));
		
		float specularFactor = dot(EyetoFrag, reflectedLight);
		
		if (specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.Shininess);
			specularColor = vec4(directionalLight.color * material.specularIntensity * specularFactor, 1.0f);
		}
	}
	
	gl_FragColor = texture(textureSampler, texelCoord) * (ambientColor + diffuseColor + specularColor);
}