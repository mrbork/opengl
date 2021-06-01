#include "pch.h"
#include "Lighting.h"

Lighting::Lighting()
{
	ambientColor = glm::vec3( 1.0f , 1.0f , 1.0f );
	ambientIntensity = 1.0f;

	direction = glm::vec3( 0.0f , -1.0f , 0.0f );
	ambientIntensity = 0.0f;
}

Lighting::Lighting( GLfloat red , GLfloat green , GLfloat blue , GLfloat ambientintensity , GLfloat dirX , GLfloat dirY , GLfloat dirZ , GLfloat diffuseintensity )
{
	ambientColor = glm::vec3( red , green , blue );
	ambientIntensity = ambientintensity;

	direction = glm::vec3( dirX , dirY , dirZ );
	diffuseIntensity = diffuseintensity;
}

void Lighting::Use( unsigned int ambientColorLocation , unsigned int ambientIntensityLocation, unsigned int directionLocation , unsigned int diffuseIntensityLocation )
{
	glUniform3f( ambientColorLocation , ambientColor.r , ambientColor.g , ambientColor.b );
	glUniform1f( ambientIntensityLocation , ambientIntensity );
	glUniform3f( directionLocation , direction.x, direction.y, direction.z );
	glUniform1f( diffuseIntensityLocation , diffuseIntensity );
}

void Lighting::Update( glm::vec3 ambientColor , float ambientIntensity )
{
	this->ambientColor = ambientColor;
	this->ambientIntensity = ambientIntensity;
}

Lighting::~Lighting()
{

}