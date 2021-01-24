#include "Homepage.h"
#include "ResourceManager.h"
#include "TextureRenderer.h"

#include<GLFW/include/glfw3.h>

#include <vector>
#include "TileGroup.h"

TextureRenderer *Renderer;
TextRenderer *TextRend;

Tile *sampleTile;

Homepage::Homepage(unsigned int width, unsigned int height, unsigned int targetResolutionWidth, unsigned int targetResolutionHeight, float tileGroupYSpace, float maxInputDelay) : State(HOMEPAGE_LOADING), Width(width),Height(height),Keys(),ResolutionWidth(targetResolutionWidth),ResolutionHeight(targetResolutionHeight),TileGroups(),TileGroupYSpace(tileGroupYSpace),CurrentColumnSelection(0),CurrentRowSelection(0),MaxInputDelay(maxInputDelay)
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
	ResourceManager::LoadTexture("textures/DisneyTitleBackground.jpg", false, "DisneyTitleBackground");
	TextRend = new TextRenderer(this->Width,this->Height);
	TextRend->Load("fonts/HELN.ttf",24);
	Renderer->DrawTexture(ResourceManager::GetTexture("DisneyTitle"),
		glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height));
}
void Homepage::Init()
{

    this->State = HOMEPAGE_LOADING;
	ResourceManager::PrepareHompageData("https://cd-static.bamgrid.com/dp-117731241344/home.json");

	float xScale = ResolutionWidth / Width;
	float yScale = ResolutionHeight / Height;
	PopulateTileGroups(xScale,yScale);

	InitializeTileGroupPositions();
	
	this->State = HOMEPAGE_ACTIVE;
}
void Homepage::ProcessInput(float dt)
{
	if (this->State == HOMEPAGE_ACTIVE)
	{
	//fUgly input handling, need to explore event driven methods

	    this->timeSinceLastInput += dt;
		if (this->Keys[GLFW_KEY_D])
		{
			if (this->CurrentColumnSelection < 5 && this->timeSinceLastInput >= this->MaxInputDelay)
			{
				CurrentColumnSelection++;
				timeSinceLastInput = 0;
			}
		}
		if (this->Keys[GLFW_KEY_A])
		{
			if (this->CurrentColumnSelection > 0 && this->timeSinceLastInput >= this->MaxInputDelay)
			{
				CurrentColumnSelection--;
				timeSinceLastInput = 0;
			}
		}
		if (this->Keys[GLFW_KEY_W])
		{
			if (this->CurrentRowSelection > 0 && this->timeSinceLastInput >= this->MaxInputDelay)
			{
				CurrentRowSelection--;
				timeSinceLastInput = 0;
			}
		}
		if (this->Keys[GLFW_KEY_S])
		{
			if (this->CurrentRowSelection < 3 && this->timeSinceLastInput >= this->MaxInputDelay)
			{
				CurrentRowSelection++;
				timeSinceLastInput = 0;
			}
		}
		if (this->Keys[GLFW_KEY_SPACE])
		{

		}
	}

}
void Homepage::Update(float dt)
{	
// Nested for loop.../slap
// Perhaps make a 1D array with hash for 2D mapping..but later
	for (auto& tg : TileGroups)
	{
		for (auto& tile : tg.Tiles)
		{
			if (tg.RowPosition == CurrentRowSelection && tile.ColumnPosition == CurrentColumnSelection)
			{
				tile.DisplaySize = tile.SelectedSize;
			}
			else
			{
				tile.DisplaySize = tile.Size;
			}
		}
		
	}
}
void Homepage::InitializeTileGroupPositions()
{
	for (int i = 0; i < TileGroups.size(); i++)
	{
		TileGroup& tg = TileGroups[i];

		float xPos = 0;
		float yPosOffset = (TileGroupYSpace * i)+100.0f;
		
		float yPos = (tg.RowPosition * tg.TileHeight) + yPosOffset;
		tg.SetTileGroupPosition(xPos,yPos);
		tg.SetTilePosition();
	}
}
void Homepage::Render()
{
	
	if (this->State == HOMEPAGE_ACTIVE)
	{
		Renderer->DrawTexture(ResourceManager::GetTexture("DisneyTitleBackground"),
			glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height));

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