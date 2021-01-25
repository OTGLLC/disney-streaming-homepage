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
#include "HomepageConfiguration.h"


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
	static std::map<std::string, Shader>					Shaders;
	static std::map<std::string, Texture>					Textures;
	static std::map<std::string, std::vector<HomepageImage>> HomepageElements;
	 //loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
	static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);

	static Shader GetShader(std::string name);

	static Texture LoadTexture(const char* file, bool alpha, std::string name);
	
	static Texture& GetTexture(std::string name);

	static void PrepareHompageData(const char* url, const char* aspectRatio);
	
	static void Clear();

	static HomepageConfiguration LoadHomepageConfiguration();
private:
	
	ResourceManager() { }
	
	static Shader    loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
	
	static Texture loadTextureFromFile(const char* file, bool alpha);

	static void DownloadImageFromURL(const char* url, std::string imageName );

	static size_t WriteCallback(void *ptr, size_t size, size_t nmemb, void* data);

	static std::stringstream GetHomepageJSON(const char* url);

	static void ParseHomepageJson(std::stringstream& jsonStream, const char* aspectRatio);

	static void GetRefContainerDetails(const rapidjson::Value& containerSet, const char* aspectRatio);

	static void GetContainerDetails(const rapidjson::Value& containerSet, const char* aspectRatio);
	
	static void DownloadTile(const rapidjson::Value& itemContainer, std::vector<HomepageImage>& imageRecord,const char* urlKey, const char* imageKey,const char* aspectRatio);

};

#endif