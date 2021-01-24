#ifndef TILE_H
#define TILE_H

#include "DisplayObject.h"
#include <glm/glm.hpp>
#include "Texture.h";
#include "TextureRenderer.h"

class Tile : DisplayObject
{
     public:
	     int ColumnPosition;
		 std::string TileName;
		 glm::vec2   Position, Size, Velocity;
		 Texture  Image;
		 Tile();
		 Tile(std::string name,glm::vec2 pos, glm::vec2 size, Texture image,int rowPosition ,glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
		 virtual void Draw(TextureRenderer& renderer) override;
};


#endif
