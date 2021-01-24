#include "DisplayObject.h"

DisplayObject::DisplayObject() :Position(0.0f, 0.0f), Size(1.0f, 1.0f), Velocity(0.0f), Rotation(0.0f), Image()
{

}
DisplayObject::DisplayObject(glm::vec2 pos, glm::vec2 size, Texture image, glm::vec2 velocity) :
	Position(pos), Size(size), Velocity(velocity), Rotation(0.0f), Image(image)
{

}
void DisplayObject::Draw(TextureRenderer& renderer)
{
	renderer.DrawTexture(this->Image,this->Position,this->Size);
}