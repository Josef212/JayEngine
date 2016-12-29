#include "Globals.h"
#include "Application.h"
#include "ImporterTexture.h"

#include "ResourceTexture.h"
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

bool ImporterTexture::import(const char* originalFile, std::string& exportedFile, UID& resUID)
{
	bool ret = false;

	std::string file, ext, origin;
	app->fs->splitPath(originalFile, NULL, &file, &ext);
	origin.assign(DEFAULT_TEXTURES_PATH + file);

	char* buffer = NULL;
	uint size = app->fs->load(origin.c_str(), &buffer);

	if (buffer && size > 0)
		ret = importBuf(buffer, size, exportedFile, resUID);

	RELEASE_ARRAY(buffer);

	if (!ret)
		_LOG(LOG_ERROR, "Could not load texture %s.", originalFile);

	return ret;
}

bool ImporterTexture::importBuf(const void* buffer, uint size, std::string& exportedFile, UID& resUID)
{
	bool ret = false;

	/**First load the image */
	//1-Gen the image and bind it
	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);

	//2-Load the image from buffer
	if (ilLoadL(IL_TYPE_UNKNOWN, buffer, size))
	{
		ilEnable(IL_FILE_OVERWRITE);

		ILuint _size;
		ILubyte* data = NULL;

		//3-Set format (DDS, DDS compression)
		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
		//4-Get size
		_size = ilSaveL(IL_DDS, NULL, 0);
		if (_size > 0)
		{
			//5-If size is more than 0 create the image buffer
			data = new ILubyte[_size];

			//6-Save the image with the correct format and save it with file system
			if (ilSaveL(IL_DDS, data, _size) > 0)
			{
				//Save it FS
				resUID = app->resourceManager->getNewUID();
				exportedFile.assign(std::to_string(resUID) + TEXTURE_EXTENSION);
				char savePath[256];
				sprintf_s(savePath, 256, "%s%s", DEFAULT_TEXTURE_SAVE_PATH, exportedFile.c_str());

				if (app->fs->save(savePath, (const char*)data, _size) == _size)
					ret = true;
				else
					_LOG(LOG_ERROR, "Error importing texture.");
			}

			//7-Release the image buffer to avoid memory leaks
			RELEASE_ARRAY(data);
		}

		//8-Finally if the image was loaded destroy the image to avoid more memory leaks
		ilDeleteImages(1, &image);
	}

	return ret;
}

bool ImporterTexture::loadResource(ResourceTexture* resource)
{
	bool ret = false;

	if (!resource)
		return ret;

	if (resource->exportedFile.empty())
		return ret;

	std::string path(DEFAULT_TEXTURE_SAVE_PATH);
	path.append(resource->getExportedFile());

	_LOG(LOG_INFO, "Loading textures resource from: '%s'.", path.c_str());

	char* data = NULL;
	uint size = app->fs->load(path.c_str(), &data);

	if (data && size > 0)
	{
		ILuint image = 0;
		ilGenImages(1, &image);
		ilBindImage(image);

		if (ilLoadL(IL_DDS, (const void*)data, size))
		{
			resource->textureGlID = ilutGLBindTexImage();
			ilDeleteImages(1, &image);

			ret = true;
		}
		
	}

	RELEASE_ARRAY(data);

	return ret;
}