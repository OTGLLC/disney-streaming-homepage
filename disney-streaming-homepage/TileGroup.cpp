#include<vector>

#include "TileGroup.h"
#include "TextRenderer.h"
#include "TextRenderer.h"

TileGroup::TileGroup(std::vector<Tile>& sourceTiles,const char* tileGroupName, int rowPosition, float tileXSeperation, float tileHeight) :Tiles(sourceTiles), TileGroupName(tileGroupName), RowPosition(rowPosition),xPosition(0),yPosition(0),TileHeight(tileHeight),TileXSeperation(tileXSeperation)
{

}

void TileGroup::SetTileGroupPosition(float xPos, float yPos)
{
	xPosition = xPos;
	yPosition = yPos;
}
void TileGroup::SetTilePosition()
{
	
	for (auto& tile : Tiles)
	{  
		float tileXpos = xPosition+(tile.ColumnPosition * tile.Size.x) + (TileXSeperation * tile.ColumnPosition);
	   tile.Position = glm::vec2(tileXpos,yPosition);
	}
}
void TileGroup::DrawTiles(TextureRenderer& renderer)
{
	for (auto& tile : Tiles)
	{
		tile.Draw(renderer);
	}
}
void TileGroup::DrawText(TextRenderer& textRend)
{
     textRend.RenderText(this->TileGroupName,this->xPosition,this->yPosition - 50., 1.0f);
}
