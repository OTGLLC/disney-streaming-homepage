#ifndef DISPLAYOBJECT_H
#define DISPLAYOBJECT_H

#include <glm/glm.hpp>

#include "Texture.h"
#include "TextureRenderer.h"

// Base class for any type of visible object within the app
class DisplayObject
{
	public:
	glm::vec2 Position, Size, Velocity;
	float Rotation;

	Texture Image;
	DisplayObject();
	DisplayObject(glm::vec2 pos, glm::vec2 size, Texture image,glm::vec2 velocity = glm::vec2(0.0f,0.0f));

	virtual void Draw(TextureRenderer &renderer);

};

#endif
