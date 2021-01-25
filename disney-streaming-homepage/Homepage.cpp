#include "Homepage.h"
#include "ResourceManager.h"
#include "TextureRenderer.h"

#include<GLFW/include/glfw3.h>

#include <vector>
#include "TileGroup.h"

TextureRenderer *Renderer;
TextRenderer *TextRend;

Tile *selectedTile;

Homepage::~Homepage()
{
      delete Renderer;
	  delete TextRend;
}
void Homepage::LoadConfig(const HomepageConfiguration& config)
{
	this->Width = config.ScreenWidth;
	this->Height = config.ScreenHeight;
	this->ResolutionWidth = config.ResolutionWidth;
	this->ResolutionHeight = config.ResolutionHeight;
	this->TileGroupYSpace = config.TileYSpace;
	this->MaxInputDelay = config.MaxInputDelay;
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
	ResourceManager::LoadTexture("textures/SelectionBox.png", true, "SelectionBox");
	TextRend = new TextRenderer(this->Width,this->Height);
	TextRend->Load("fonts/HELN.ttf",24);
	Renderer->DrawTexture(ResourceManager::GetTexture("DisneyTitle"),
		glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height));

	
	
}
void Homepage::Init()
{

    this->State = HOMEPAGE_LOADING;
	ResourceManager::PrepareHompageData("https://cd-static.bamgrid.com/dp-117731241344/home.json", "1.78");

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
				selectedTile = &tile;
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
	//TODO: Implement a proper FSM to handle app state and transitions
	if (this->State == HOMEPAGE_ACTIVE)
	{
		Renderer->DrawTexture(ResourceManager::GetTexture("DisneyTitleBackground"),
			glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height));

	    //TODO: Handler tile scale and border with a shader
		if (selectedTile != nullptr)
		{
		    glm::vec2 pos(selectedTile->Position.x -1.5,selectedTile->Position.y - 1.5f);
			Renderer->DrawTexture(ResourceManager::GetTexture("SelectionBox"),
				pos,
				selectedTile->Size*1.12f);
			
		}

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