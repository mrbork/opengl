#include "pch.h"
#include "Material.h"

Material::Material()
{
	specularIntensity = 0.0f;
	Shininess = 0.0f;
}

Material::Material( GLfloat specularintensity , GLfloat shininess )
{
	specularIntensity = specularintensity;
	Shininess = shininess;
}

void Material::UseMaterial( GLuint specularIntensityLocation , GLuint ShininessLocation )
{
	glUniform1f( specularIntensityLocation , specularIntensity );
	glUniform1f( ShininessLocation , Shininess );
}

Material::~Material()
{

}