#ifndef TILE_GROUP_H
#define TILE_GROUP_H

#include <vector>

#include"Tile.h"
#include "TextRenderer.h"

class TileGroup
{
	public:
		float xPosition;
		float yPosition;
	  int RowPosition;
	  float TileHeight;
	  const char* TileGroupName;
	  std::vector<Tile> Tiles;
	  float TileXSeperation;

	  TileGroup(std::vector<Tile>& sourceTiles, const char* tileGroupName, int rowPosition, float tileXSeperation, float tileHeight);
	  void SetTilePosition();
	  void SetTileGroupPosition(float xPos, float yPos);
	  void DrawTiles(TextureRenderer& renderer);
	  void DrawText(TextRenderer& textRend, float& textYPositionOffset);

	
	  
};
#endif
