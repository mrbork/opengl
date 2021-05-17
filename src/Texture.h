#include <GL/glew.h>

#include "stb_image.h"


class Texture
{
public:
	Texture();
	Texture( const char* fileLoc );

	void Load();
	void Use();
	void Clear();

	~Texture();

private:
	unsigned textureID;
	const char* fileLocation;
	int width;
	int height;
	int bitDepth;

};

