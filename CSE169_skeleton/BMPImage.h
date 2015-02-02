#include "../core.h"
class BMPImage
{
public:
	~BMPImage()
	{
		delete[] data;
	}
	GLuint texture[1];
	bool load(const char* filename);
	void LoadGLTextures(const char* filename);
	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
};
