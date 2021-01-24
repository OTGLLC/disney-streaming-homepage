#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <vector>
#include <string>
#include <ostream>

#include <glad/include/glad/glad.h>
#include "rapidjson/document.h"
#include "Texture.h"
#include "Shader.h"




// A static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no 
// public constructor is defined.
class ResourceManager
{
	struct HomepageImage
	{
		public:
		std::string ImageName;
		std::string ImagePath;
		std::string ImageUrl;

	};
public:
	// resource storage
	static std::map<std::string, Shader>    Shaders;
	static std::map<std::string, Texture> Textures;
	static std::map<std::string, std::vector<HomepageImage>> HomepageElements;
	 //loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
	static Shader    LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
	// retrieves a stored sader
	static Shader    GetShader(std::string name);
	// loads (and generates) a texture from file
	static Texture LoadTexture(const char* file, bool alpha, std::string name);
	// retrieves a stored texture
	static Texture& GetTexture(std::string name);

	static void PrepareHompageData(const char* url);
	// properly de-allocates all loaded resources
	static void      Clear();
private:
	// private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
	ResourceManager() { }
	// loads and generates a shader from file
	static Shader    loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
	// loads a single texture from file
	static Texture loadTextureFromFile(const char* file, bool alpha);
	static void DownloadImageFromURL(const char* url, std::string imageName );
	static size_t WriteCallback(void *ptr, size_t size, size_t nmemb, void* data);
	static std::stringstream GetHomepageJSON(const char* url);
	static void ParseHomepageJson(std::stringstream& jsonStream);
	static void GetRefContainerDetails(const rapidjson::Value& containerSet);
	static void GetTilesFromContainerSet(const rapidjson::Value& containerSet);
	

};

#endif