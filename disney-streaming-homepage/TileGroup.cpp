#include<vector>

#include "TileGroup.h"

TileGroup::TileGroup(std::vector<Tile>& sourceTiles,const char* tileGroupName) :Tiles(sourceTiles), TileGroupName(tileGroupName)
{

}
