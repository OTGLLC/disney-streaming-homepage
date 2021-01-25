#include "ResourceManager.h"

#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "Macros.h"
#include "stb_image.h"
#include "curl/include/curl/curl.h"
#include "curlcpp/include/curl_easy.h"
#include "curlcpp/include/curl_form.h"
#include "curlcpp/include/curl_ios.h"
#include "curlcpp/include/curl_exception.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


const char* jkey_series = "series";
const char* jkey_tile = "tile";
const char* jkey_items = "items";
const char* jkey_meta = "meta";
const char* jkey_page_size = "page_size";
const char* jkey_set = "set";
const char* jkey_text = "text";
const char* jkey_title = "title";
const char* jkey_full = "full";
const char* jkey_collection = "collection";
const char* jkey_image = "image";
const char* jkey_default = "default";
const char* jkey_url = "url";
const char* jkey_program = "program";
const char* jkey_content = "content";
const char* jkey_data = "data";
const char* jkey_StandardCollection = "StandardCollection";
const char* jkey_refId = "refId";
const char* jkey_containers = "containers";






std::map<std::string, Texture>					ResourceManager::Textures;
std::map<std::string, Shader>					ResourceManager::Shaders;
std::map<std::string, std::vector<ResourceManager::HomepageImage>> ResourceManager::HomepageElements;


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
	
	for(auto& iter : Shaders)
		glDeleteProgram(iter.second.ID);
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try
	{
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();

		vertexShaderFile.close();
		fragmentShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

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
		OTG_LOG("ERROR::SHADER: Failed to read shader files");
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geometryCode.c_str();

	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}

Texture ResourceManager::loadTextureFromFile(const char* file, bool alpha)
{

	Texture texture;
	if (alpha)
	{
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}

	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

	texture.Generate(width, height, data);

	stbi_image_free(data);
	return texture;
}

void ResourceManager::DownloadImageFromURL(const char* url, std::string imageName)
{
	FILE* fp = fopen(imageName.c_str(),"wb");
	if (!fp)
	{
		OTG_LOG("Failed to create file on disk");
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
	    OTG_LOG("!! Failed to download: " << url);
		return;
	}

	long res_code = 0;
	curl_easy_getinfo(curlCtx,CURLINFO_RESPONSE_CODE, &res_code);
	if (!((res_code == 200 || res_code == 201) && rc != CURLE_ABORTED_BY_CALLBACK))
	{
		OTG_LOG("!!!Response code:"<< res_code);
		return;
	}

	if (res_code == 404 && rc != CURLE_ABORTED_BY_CALLBACK)
	{
	    OTG_LOG("Cant download image..404");
		return;
	}

	curl_easy_cleanup(curlCtx);
	fclose(fp);

	Textures[imageName] = loadTextureFromFile(imageName.c_str(), false);
}

size_t ResourceManager::WriteCallback(void* ptr, size_t size, size_t nmemb, void* data)
{
	FILE* stream = (FILE*)data;
	if (!stream)
	{
		OTG_LOG(" No File Stream");
		return 0;
	}

	size_t written = fwrite((FILE*)ptr,size,nmemb,stream);

	return written;
}

void ResourceManager::PrepareHompageData(const char* url, const char* aspectRatio)
{
	auto homePageResponse = GetHomepageJSON(url);
	ParseHomepageJson(homePageResponse, aspectRatio);
	
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

void ResourceManager::ParseHomepageJson(std::stringstream& jsonStream, const char* aspectRatio)
{	
	using namespace rapidjson;
	Document d;

	d.Parse(jsonStream.str().c_str());
	
	const Value& containers = d[jkey_data][jkey_StandardCollection][jkey_containers];

	assert(containers.IsArray());
	for (SizeType i = 0; i < containers.Size(); i++)
	{
	      const Value& containerSet = containers[i][jkey_set];
		  auto refId = containerSet.HasMember(jkey_refId);

		  if (!refId)
		  {
			  GetContainerDetails(containerSet, aspectRatio);
		  }
		  else
		  {
			 GetRefContainerDetails(containerSet,aspectRatio);
		  }
		  
	}

	
}

void ResourceManager::GetRefContainerDetails(const rapidjson::Value& containerSet, const char* aspectRatio)
{
	using namespace rapidjson;

	const Value& refId = containerSet[jkey_refId];
	const Value& refName = containerSet[jkey_text][jkey_title][jkey_full][jkey_set][jkey_default][jkey_content];

	OTG_LOG(refName.GetString()<<" : "<<refId.GetString());

}

/// <summary>
/// Yep..its ugly..i dont like it but time is short
/// Maybe research dynamic serialization.....ugh
/// </summary>
/// <param name="containerSet"></param>
void ResourceManager::GetContainerDetails(const rapidjson::Value& containerSet, const char* aspectRatio)
{
	using namespace rapidjson;

	std::vector<HomepageImage> hpImages;

    const Value& items = containerSet[jkey_items];
	const Value& pageSize = containerSet[jkey_meta][jkey_page_size];
	const Value& containerTitle = containerSet[jkey_text][jkey_title][jkey_full][jkey_set][jkey_default][jkey_content];
	

	for (SizeType i = 0; i < items.Size(); i++)
	{
		
		const Value& itemParent = items[i];
		
		if(itemParent[jkey_text][jkey_title][jkey_full].HasMember(jkey_program))
		{
			DownloadTile(items[i], hpImages, jkey_program, jkey_program, aspectRatio);
		}
		else if (itemParent[jkey_text][jkey_title][jkey_full].HasMember(jkey_collection))
		{
			DownloadTile(items[i], hpImages, jkey_default, jkey_collection, aspectRatio);
		}
		else
		{
			DownloadTile(items[i], hpImages, jkey_series, jkey_series,aspectRatio);
		}

	}
	HomepageElements[containerTitle.GetString()] = hpImages;

}

void ResourceManager::DownloadTile(const rapidjson::Value& itemContainer, std::vector<HomepageImage>& imageRecord,const char* urlKey, const char* imageKey, const char* aspectRatio)
{
	using namespace rapidjson;

	const Value& imageUrl = itemContainer[jkey_image][jkey_tile][aspectRatio][urlKey][jkey_default][jkey_url];
	const Value& titleName = itemContainer[jkey_text][jkey_title][jkey_full][imageKey][jkey_default][jkey_content];

	std::string name(titleName.GetString());
	std::string filePath("textures/" + name + ".jpg");
	std::string url(imageUrl.GetString());

	HomepageImage img{ name,filePath,url };

	imageRecord.push_back(img);

	DownloadImageFromURL(imageUrl.GetString(), filePath);

	OTG_LOG(titleName.GetString() << " : " << imageUrl.GetString());
}
