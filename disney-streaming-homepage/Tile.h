#ifndef TILE_H
#define TILE_H

#include "DisplayObject.h"

class Tile : DisplayObject
{
     public:
		 const char* ImageUrl;
		 const char* TileTitle;
     Tile(const char* imageUrl, const char* tileTitle);
     
     private:
     
};


#endif
