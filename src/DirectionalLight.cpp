#include "pch.h"
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() 
	: Lighting()
{
	direction = glm::vec3( 0.0f , -1.0f , 0.0f );
}

DirectionalLight::DirectionalLight( GLfloat red , GLfloat green , GLfloat blue , GLfloat ambientIntensity , GLfloat diffuseIntensity , GLfloat dirX , GLfloat dirY , GLfloat dirZ )
	: Lighting(red,green, blue, ambientIntensity, diffuseIntensity )
{
	direction = glm::vec3( dirX , dirY , dirZ );
}

void DirectionalLight::Use( unsigned int ambientColorLocation , unsigned int ambientIntensityLocation , unsigned int diffuseIntensityLocation , unsigned int directionLocation )
{
	glUniform3f( ambientColorLocation , ambientColor.r , ambientColor.g , ambientColor.b );
	glUniform1f( ambientIntensityLocation , ambientIntensity );
	glUniform1f( diffuseIntensityLocation , diffuseIntensity );
	glUniform3f( directionLocation , direction.x, direction.y, direction.z );
}

DirectionalLight::~DirectionalLight()
{

}