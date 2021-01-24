#include "ResourceManager.h"

#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "curl/include/curl/curl.h"

#include "curlcpp/include/curl_easy.h"
#include "curlcpp/include/curl_form.h"
#include "curlcpp/include/curl_ios.h"
#include "curlcpp/include/curl_exception.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


// Instantiate static variables
std::map<std::string, Texture>    ResourceManager::Textures;
std::map<std::string, Shader>     ResourceManager::Shaders;
std::map<std::string, std::vector<std::string>> HomepageElements;


Shader ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

Texture ResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
{
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}

Texture& ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}

void ResourceManager::Clear()
{
	//// (properly) delete all shaders	
	//for (auto iter : Shaders)
	//	glDeleteProgram(iter.second.ID);
	// (properly) delete all textures
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try
	{
		// open files
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		// if geometry shader path is present, also load a geometry shader
		if (gShaderFile != nullptr)
		{
			std::ifstream geometryShaderFile(gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geometryCode.c_str();
	// 2. now create shader object from source code
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}

Texture ResourceManager::loadTextureFromFile(const char* file, bool alpha)
{
	// create texture object
	Texture texture;
	if (alpha)
	{
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}
	// load image
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
	
	
	// now generate texture
	texture.Generate(width, height, data);
	// and finally free image data
	stbi_image_free(data);
	return texture;
}

void ResourceManager::DownloadImageFromURL(const char* url, std::string imageName)
{
	FILE* fp = fopen(imageName.c_str(),"wb");
	if (!fp)
	{
		printf("Failed to create file on disk\n");
		return;
	}
	
	CURL* curlCtx = curl_easy_init();
	curl_easy_setopt(curlCtx, CURLOPT_URL, url);
	curl_easy_setopt(curlCtx, CURLOPT_WRITEDATA, fp);
	curl_easy_setopt(curlCtx, CURLOPT_WRITEFUNCTION,WriteCallback);
	curl_easy_setopt(curlCtx, CURLOPT_FOLLOWLOCATION,1);

	CURLcode rc = curl_easy_perform(curlCtx);

	if (rc)
	{
		printf("!! Failed to download: %s\n", url);
		return;
	}

	long res_code = 0;
	curl_easy_getinfo(curlCtx,CURLINFO_RESPONSE_CODE, &res_code);
	if (!((res_code == 200 || res_code == 201) && rc != CURLE_ABORTED_BY_CALLBACK))
	{
		printf("!!!Response code: %d\n", res_code);
		return;
	}

	
	
	
	curl_easy_cleanup(curlCtx);
	fclose(fp);
	Textures[imageName] = loadTextureFromFile(imageName.c_str(), false);
	return;
}

size_t ResourceManager::WriteCallback(void* ptr, size_t size, size_t nmemb, void* data)
{
	FILE* stream = (FILE*)data;
	if (!stream)
	{
		printf("No Stream\n");
		return 0;
	}

	
	size_t written = fwrite((FILE*)ptr,size,nmemb,stream);

	return written;
}

void ResourceManager::PrepareHompageData(const char* url)
{
	auto homePageResponse = GetHomepageJSON(url);
	ParseHomepageJson(homePageResponse);
	
}

std::stringstream ResourceManager::GetHomepageJSON(const char* url)
{
	std::stringstream str;
	curl::curl_ios<std::stringstream> writer(str);

	curl::curl_easy easy(writer);

	easy.add<CURLOPT_URL>(url);
	easy.add<CURLOPT_FOLLOWLOCATION>(1L);

	try
	{
		easy.perform();
	}
	catch (curl::curl_easy_exception error)
	{
		auto errors = error.get_traceback();
		error.print_traceback();
	}

	return str;
	
}
void ResourceManager::ParseHomepageJson(std::stringstream& jsonStream)
{	
	using namespace rapidjson;
	Document d;
	d.Parse(jsonStream.str().c_str());
	
	const Value& containers = d["data"]["StandardCollection"]["containers"];
	assert(containers.IsArray());
	for (SizeType i = 0; i < containers.Size(); i++)
	{
	      const Value& containerSet = containers[i]["set"];
		  auto refId = containerSet.HasMember("refId");

		  if (!refId)
		  {
			  GetEditorialTileGroupFromContainerSet(containerSet);
			  GetTilesFromContainerSet(containerSet);
		  }
		  else
		  {
			 GetRefSetTileGroupFromContainerSet(containerSet);
		  }
		  
	}

	
}

void ResourceManager::GetEditorialTileGroupFromContainerSet(const rapidjson::Value& containerSet)
{
	using namespace rapidjson;

	const Value& pageSize = containerSet["meta"]["page_size"];
	assert(pageSize.IsInt());

	const Value& containerTitle = containerSet["text"]["title"]["full"]["set"]["default"]["content"];
	assert(containerTitle.IsString());

	
	std::cout << containerTitle.GetString() << " : "<<pageSize.GetInt() << std::endl;
	
}
void ResourceManager::GetRefSetTileGroupFromContainerSet(const rapidjson::Value& containerSet)
{
	using namespace rapidjson;

	const Value& refId = containerSet["refId"];
	const Value& refName = containerSet["text"]["title"]["full"]["set"]["default"]["content"];

	std::cout << refName.GetString() << " : " << refId.GetString() <<std::endl;

}
/// <summary>
/// Yep..its ugly..i dont like it but time is short
/// Maybe research dynamic serialization.....ugh
/// </summary>
/// <param name="containerSet"></param>
void ResourceManager::GetTilesFromContainerSet(const rapidjson::Value& containerSet)
{
	using namespace rapidjson;

     const Value& items = containerSet["items"];
	 assert(items.IsArray());

	for (SizeType i = 0; i < items.Size(); i++)
	{
		
		const Value& itemParent = items[i];
		
		

		if(itemParent["text"]["title"]["full"].HasMember("program"))
		{
			const Value& imageUrl = items[i]["image"]["tile"]["1.78"]["program"]["default"]["url"];
			const Value& titleName = items[i]["text"]["title"]["full"]["program"]["default"]["content"];
			assert(titleName.IsString());
			
			std::string str(titleName.GetString());
			std::string filePath("textures/"+str+".jpg");

			
			
			DownloadImageFromURL(imageUrl.GetString(),filePath);
			std::cout << titleName.GetString() << " : " << imageUrl.GetString() << std::endl;
		}
		else if (itemParent["text"]["title"]["full"].HasMember("collection"))
		{
			const Value& imageUrl = items[i]["image"]["tile"]["1.78"]["default"]["default"]["url"];
			const Value& titleName = items[i]["text"]["title"]["full"]["collection"]["default"]["content"];
			assert(titleName.IsString());
			std::cout << titleName.GetString() << " : " << imageUrl.GetString() << std::endl;
		}
		else
		{
			const Value& imageUrl = items[i]["image"]["tile"]["1.78"]["series"]["default"]["url"];
			const Value& titleName = items[i]["text"]["title"]["full"]["series"]["default"]["content"];
			assert(titleName.IsString());
			std::string str(titleName.GetString());
			std::string filePath("textures/" + str + ".jpg");

			DownloadImageFromURL(imageUrl.GetString(), filePath);
			std::cout << titleName.GetString() << " : " << imageUrl.GetString() << std::endl;
		}
		

		

		
	}

}
