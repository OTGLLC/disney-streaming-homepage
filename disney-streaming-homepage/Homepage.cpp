#include "Homepage.h"
#include "ResourceManager.h"
#include "TextureRenderer.h"

#include <vector>
#include "TileGroup.h"

TextureRenderer *Renderer;
TextRenderer *TextRend;

Tile *sampleTile;

Homepage::Homepage(unsigned int width, unsigned int height, unsigned int targetResolutionWidth, unsigned int targetResolutionHeight, float tileGroupYSpace) : State(HOMEPAGE_LOADING), Width(width),Height(height),Keys(),ResolutionWidth(targetResolutionWidth),ResolutionHeight(targetResolutionHeight),TileGroups(),TileGroupYSpace(tileGroupYSpace)
{
      
}
Homepage::~Homepage()
{
      delete Renderer;
}
void Homepage::DrawSplashScreen()
{
	ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
		static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	Renderer = new TextureRenderer(ResourceManager::GetShader("sprite"));
	ResourceManager::LoadTexture("textures/DisneyTitle.jpg", false, "DisneyTitle");
	TextRend = new TextRenderer(this->Width,this->Height);
	TextRend->Load("fonts/HELN.ttf",24);
	Renderer->DrawTexture(ResourceManager::GetTexture("DisneyTitle"),
		glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height));
}
void Homepage::Init()
{

	ResourceManager::PrepareHompageData("https://cd-static.bamgrid.com/dp-117731241344/home.json");

	float xScale = ResolutionWidth / Width;
	float yScale = ResolutionHeight / Height;
	PopulateTileGroups(xScale,yScale);

	InitializeTileGroupPositions();
	///*ResourceManager::LoadTexture("textures/Cool Runnings.jpg",false,"Cool Runnings");
	//Texture t = ResourceManager::GetTexture("Cool Runnings");

	//sampleTile = new Tile("Cool Runni*/ngs",glm::vec2(0.0f,0.0f),glm::vec2(t.Width,t.Height),t);

	this->State = HOMEPAGE_ACTIVE;
}
void Homepage::ProcessInput(float dt)
{

}
void Homepage::Update(float dt)
{	

}
void Homepage::InitializeTileGroupPositions()
{
	for (int i = 0; i < TileGroups.size(); i++)
	{
		TileGroup& tg = TileGroups[i];

		float xPos = 0;
		float yPosOffset = TileGroupYSpace * i;
		
		float yPos = (tg.RowPosition * tg.TileHeight) + yPosOffset;
		tg.SetTileGroupPosition(xPos,yPos);
		tg.SetTilePosition();
	}
}
void Homepage::Render()
{
	if (this->State == HOMEPAGE_ACTIVE)
	{
		for (auto& tg : TileGroups)
		{
			tg.DrawText(*TextRend);
			tg.DrawTiles(*Renderer);
		}

	}
	
}
void Homepage::PopulateTileGroups(float xScale, float yScale)
{
    int tileGroupRowPosition(0);
	int tileColumnPosition(0);

	for (auto const& x : ResourceManager::HomepageElements)
	{
		std::vector<Tile> tiles;
		float tileHeight = 0;
		for (auto const& image : x.second)
		{
			Texture texture = ResourceManager::GetTexture(image.ImagePath);
			Tile t = Tile(image.ImageName,glm::vec2(0.0f,0.0f),glm::vec2(texture.Width/xScale,texture.Height/yScale),texture,tileColumnPosition);
			tileHeight = t.Size.y;
			tiles.push_back(t);
			tileColumnPosition++;
		}
		
		TileGroup tg = TileGroup(tiles,x.first.c_str(),tileGroupRowPosition,80.0f,tileHeight);
		TileGroups.push_back(tg);
		tileGroupRowPosition++;
		tileColumnPosition = 0;
	}
}