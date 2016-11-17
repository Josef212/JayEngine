#include "Globals.h"
#include "Application.h"
#include "ImporterTexture.h"

#include "ModuleResourceManager.h"
#include "ModuleFileSystem.h"

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#pragma comment(lib, "Devil/libx86/DevIL.lib")
#pragma comment(lib, "Devil/libx86/ILU.lib")
#pragma comment(lib, "Devil/libx86/ILUT.lib")


ImporterTexture::ImporterTexture() : Importer()
{
	_LOG(LOG_STD, "Creating a texture importer.");

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	ILuint devilError = ilGetError();
	if (devilError != IL_NO_ERROR)
	{
		_LOG(LOG_ERROR, "Error while Devil Init: %s\n", iluErrorString(devilError));
	}
}


ImporterTexture::~ImporterTexture()
{
	ilShutDown();
}

void ImporterTexture::importTexture(const char* fileName, std::string& outputName) //Note in texture case the output name will be a new UID + extension
{//TODO: outputName should be the original file with all the path? The new file? Which extension should have the outputName
	if (!fileName)
	{
		_LOG(LOG_ERROR, "Error loading texture: invalid file name.");
		return;
	}

	//TODO: Assign the path directly here? Assume file name is clear?... For now the fileName is clear and the path is added here
	outputName.assign(DEFAULT_TEXTURES_PATH);
	outputName.append("/");
	outputName.append(fileName);

	_LOG(LOG_INFO, "Loading a new texture: %s.", outputName.c_str());

	char* buffer;
	uint size = app->fs->load(outputName.c_str(), &buffer);

	if (buffer && size > 0)
	{
		//First load the image with unknown format //TODO: maybe identify forst the file format and load the file with the correct format
		//For now will use UNKNOWN parameter
		//1-Gen the image and bind it
		ILuint image;
		ilGenImages(1, &image);
		ilBindImage(image);

		//TODO:
		/*ILenum format;
		switch (texFormat)
		{
		default: format = IL_TYPE_UNKNOWN;
			break;
		}*/

		//2-Load the image from buffer
		if (ilLoadL(IL_TYPE_UNKNOWN, buffer, size))
		{
			//Check overwitting parameter (ilEnable). if textuer already exist maybe shoudl overwrite it??
			
			ILubyte* data;
			ILuint size;
			//3-Set format (DDS, DDS compression)
			ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
			//4-Get size
			size = ilSaveL(IL_DDS, NULL, 0);
			if ( size > 0)
			{
				//5-If size is more than 0 create the image buffer
				data = new ILubyte[size];

				//6-Save the image with the correct format and save it with file system
				if (ilSaveL(IL_DDS, data, size) > 0)
				{
					std::string savePath(DEFAULT_TEXTURE_SAVE_PATH + std::to_string(app->resourceManager->getNewUID()) + TEXTURE_EXTENSION);
					if (app->fs->save(savePath.c_str(), (const char*)data, size) != size)
					{
						_LOG(LOG_ERROR, "ERROR saving the dds texture.");
					}
					else
					{
						_LOG(LOG_INFO, "New dds created: %s.", savePath.c_str());
					}
				}

				//7-Release the image buffer to avoid memory leaks
				RELEASE_ARRAY(data);
			}

			//8-Finally if the image was loaded destroy the image to avoid more memory leaks
			ilDeleteImages(1, &image);
		}
		else
			_LOG(LOG_ERROR, "Could not load the image: %s.", outputName.c_str());
		
	}
	else
		_LOG(LOG_ERROR, "Could not load the image: %s.", outputName.c_str());

	RELEASE_ARRAY(buffer);
}