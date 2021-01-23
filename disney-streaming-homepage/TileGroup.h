#ifndef TILE_GROUP_H
#define TILE_GROUP_H

#include <vector>

#include"Tile.h"

class TileGroup
{
	public:
	  const char* TileGroupName;
	  std::vector<Tile> Tiles;

	  TileGroup(std::vector<Tile>& sourceTiles, const char* tileGroupName);
};
#endif
