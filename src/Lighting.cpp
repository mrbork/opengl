#include "Lighting.h"

Lighting::Lighting()
{
	color = glm::vec3( 1.0f , 1.0f , 1.0f );
	intensity = 1.0f;
}

Lighting::Lighting( GLfloat red , GLfloat green , GLfloat blue , GLfloat ambientIntensity )
{
	color = glm::vec3( red , green , blue );
	intensity = ambientIntensity;
}

void Lighting::Use( unsigned int ambientColorLocation , unsigned int ambientIntensityLocation )
{
	glUniform3f( ambientColorLocation , color.r , color.g , color.b );
	glUniform1f( ambientIntensityLocation , intensity );
}

void Lighting::Update( glm::vec3 ambientColor , float ambientIntensity )
{
	color = ambientColor;
	intensity = ambientIntensity;
}

Lighting::~Lighting()
{

}