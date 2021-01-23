#include "Homepage.h"
#include "ResourceManager.h"
#include "TextureRenderer.h"

#include <vector>
#include "TileGroup.h"

TextureRenderer *Renderer;

Homepage::Homepage(unsigned int width, unsigned int height) : State(PAGE_LOAD), Width(width),Height(height),Keys()
{

}
Homepage::~Homepage()
{
      delete Renderer;
}
void Homepage::Init()
{
	// load shaders
	ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
	// configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
		static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	// set render-specific controls
	Renderer = new TextureRenderer(ResourceManager::GetShader("sprite"));
	// load textures
	ResourceManager::LoadTexture("textures/awesomeface.png", true, "face");
}
void Homepage::ProcessInput(float dt)
{

}
void Homepage::Update(float dt)
{

}
void Homepage::Render()
{
	Renderer->DrawTexture(ResourceManager::GetTexture("face"), glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}
void Homepage::PopulateTileGroups(std::vector<TileGroup> tileGroups)
{
	TileGroups = tileGroups;
}