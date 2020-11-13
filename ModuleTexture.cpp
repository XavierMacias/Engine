#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleProgram.h"
#include "DevIL/include/IL/il.h"

ModuleTexture::ModuleTexture()
{

}

// Destructor
ModuleTexture::~ModuleTexture()
{
}

// Called before render is available
bool ModuleTexture::Init()
{
	ilInit();
	iluInit();

	ilGenImages(1, &texid); 
	ilBindImage(texid);

	ILboolean success;
	ILenum Error;
	success = ilLoadImage("lenna.png");
	if (success == IL_TRUE) /* If no error occured: */
	{
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	}
	if (success == IL_FALSE)
	{
		/* Error occured */
		LOG("ERROR");
		return false;
	}
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
		ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
		ilGetData());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	ilDeleteImage(texid);
	return true;
}


// Called every draw update
update_status ModuleTexture::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleTexture::CleanUp()
{
	ilDeleteImage(texid);
	glDeleteTextures(1, &textureID);
	return true;

}