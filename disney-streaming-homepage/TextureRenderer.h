#ifndef TEXTURE_RENDERER_H
#define TEXTURE_RENDERER_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "Shader.h"


class TextureRenderer
{
public:
	// Constructor (inits shaders/shapes)
	TextureRenderer(Shader shader);
	// Destructor
	~TextureRenderer();
	// Renders a defined quad textured with given sprite
	void DrawTexture(Texture& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f));
private:
	// Render state
	Shader       shader;
	unsigned int quadVAO;
	// Initializes and configures the quad's buffer and vertex attributes
	void initRenderData();
};

#endif
