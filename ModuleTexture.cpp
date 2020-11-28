#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleProgram.h"
#include "ModuleEditor.h"
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

	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	width = height = 0;

	return true;
}


// Called every draw update
update_status ModuleTexture::Update()
{
	return UPDATE_CONTINUE;
}

unsigned ModuleTexture::Load(const char* file_name, const char* model_path) {
	
	ILuint texid;
	ILinfo info;

	ilGenImages(1, &texid);
	ilBindImage(texid);

	ILboolean success;
	unsigned textureID = 0;

	App->editor->AddLog("Search texture in FBX path\n");
	success = ilLoadImage(file_name);
	
	if (success == IL_FALSE)
	{
		App->editor->AddLog("Search texture in the FBX directory\n");
		std::string fullPath(model_path);
		fullPath = fullPath.substr(0, fullPath.find_last_of("\\/"));
		fullPath = fullPath + "\\" + std::string(file_name);
		success = ilLoadImage(fullPath.c_str());

		if (success == IL_FALSE)
		{
			App->editor->AddLog("Search texture in the Texture folder\n");
			std::string myPath("..\\Textures\\");
			myPath = myPath + std::string(file_name);
			success = ilLoadImage(myPath.c_str());
			if (success == IL_FALSE)
			{
				App->editor->AddLog("Texture not found...\n");
				success = ilLoadImage("black.png");

			}

		}
	}
	if (success == IL_TRUE)
	{
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		iluGetImageInfo(&info);
		if (info.Origin == IL_ORIGIN_UPPER_LEFT) {
			iluFlipImage();
		}

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
			ilGetData());

		width = ilGetInteger(IL_IMAGE_WIDTH);
		height = ilGetInteger(IL_IMAGE_HEIGHT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	

	}

	ilDeleteImage(texid);
	return textureID;
}

bool ModuleTexture::CleanUp()
{
	return true;

}

void ModuleTexture::FreeTexture(unsigned textID) {
	glDeleteTextures(1, &textID);
}