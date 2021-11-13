#include "pch.h"
#include "Lighting.h"

Lighting::Lighting()
{
	ambientColor = glm::vec3( 1.0f , 1.0f , 1.0f );

	ambientIntensity = 1.0f;
	diffuseIntensity = 0.0f;
}

Lighting::Lighting( GLfloat red , GLfloat green , GLfloat blue , GLfloat ambientintensity , GLfloat diffuseintensity )
{
	ambientColor = glm::vec3( red , green , blue );

	ambientIntensity = ambientintensity;
	diffuseIntensity = diffuseintensity;
}

void Lighting::Use( unsigned int ambientColorLocation , unsigned int ambientIntensityLocation, unsigned int diffuseIntensityLocation)
{
	glUniform3f( ambientColorLocation , ambientColor.r , ambientColor.g , ambientColor.b );
	glUniform1f( ambientIntensityLocation , ambientIntensity );
	glUniform1f( diffuseIntensityLocation , diffuseIntensity );
}

Lighting::~Lighting()
{

}