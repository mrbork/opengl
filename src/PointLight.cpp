#include "pch.h"
#include "PointLight.h"

PointLight::PointLight()
	: Lighting()
{
	position = glm::vec3( 0.0f , 0.0f , 0.0f );

	constant = 1.0f;
	linear = 1.0f;
	exponent = 1.0f;
}

PointLight::PointLight( GLfloat red , GLfloat green , GLfloat blue , GLfloat ambientIntensity , GLfloat diffuseIntensity , GLfloat posX , GLfloat posY , GLfloat posZ , GLfloat cont , GLfloat lin , GLfloat exp )
	: Lighting( red , green , blue , ambientIntensity , diffuseIntensity)
{
	position = glm::vec3( posX , posY , posZ );

	constant = cont;
	linear = lin;
	exponent = exp;
}

void PointLight::Use( unsigned int ambientColorLocation , unsigned int ambientIntensityLocation , unsigned int diffuseIntensityLocation , unsigned int positionLocation , unsigned int constantLocation , unsigned int linearLocation , unsigned int exponentLocation )
{
	glUniform3f( ambientColorLocation , ambientColor.r , ambientColor.g , ambientColor.b );
	glUniform1f( ambientIntensityLocation , ambientIntensity );
	glUniform1f( diffuseIntensityLocation , diffuseIntensity );

	glUniform3f( positionLocation , position.x , position.y , position.z );
	glUniform1f( constantLocation , constant );
	glUniform1f( linearLocation , linear );
	glUniform1f( exponentLocation , exponent );
}

PointLight::~PointLight()
{

}