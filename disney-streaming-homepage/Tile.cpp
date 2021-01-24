
#include "Tile.h"

Tile::Tile()
	: Position(0.0f, 0.0f), Size(1.0f, 1.0f), Velocity(0.0f), Image(), TileName() { }

Tile::Tile(std::string name,glm::vec2 pos, glm::vec2 size, Texture image,int rowPosition,glm::vec2 velocity)
	: TileName(name),Position(pos), Size(size), Velocity(velocity), Image(image),ColumnPosition(rowPosition) { }

void Tile::Draw(TextureRenderer& renderer)
{
    
	renderer.DrawTexture(this->Image, this->Position, this->Size);
}