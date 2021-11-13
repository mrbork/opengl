#include "pch.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture()
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}

Texture::Texture( const char* fileLoc )
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
}

void Texture::Load()
{
	stbi_set_flip_vertically_on_load( true );
	unsigned char* imagedata = stbi_load( fileLocation , &width , &height , &bitDepth , 0 );

	if ( !imagedata )
		printf( "Failed to load Texture.\n" );

	glGenTextures( 1 , &textureID );
	glBindTexture( GL_TEXTURE_2D , textureID );

	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );

	//Only supported PNGs have RGB alpha
	glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGB , width , height , 0 , GL_RGB , GL_UNSIGNED_BYTE , imagedata );
	glGenerateMipmap( GL_TEXTURE_2D );

	glBindTexture( GL_TEXTURE_2D , 0 );

	stbi_image_free( imagedata );
}

void Texture::Use()
{
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D , textureID );
}

void Texture::Clear()
{
	glDeleteTextures( 1 , &textureID );
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}

Texture::~Texture()
{
	Clear();
}