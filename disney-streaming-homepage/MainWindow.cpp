

#include <glad/include/glad/glad.h>
#include<GLFW/include/glfw3.h>


#include "Macros.h"
#include "Homepage.h"
#include "ResourceManager.h"
#include "TileGroup.h"


#include <iostream>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);


//Configurations - move to config file and load on app init
const unsigned int SCREEN_WIDTH = 1920;
const unsigned int SCREEN_HEIGHT = 1080;

const unsigned int RENDER_RESOLUION_WIDTH = 3840;
const unsigned int RENDER_RESOLUION_HEIGHT = 2160;

const float TILE_GROUP_Y_SPACE = 100.0f;
const float MAX_INPUT_DELAY = 0.3f;

float aspect = SCREEN_WIDTH / SCREEN_HEIGHT;


//
//Homepage hPage(SCREEN_WIDTH, SCREEN_HEIGHT, RENDER_RESOLUION_WIDTH, RENDER_RESOLUION_HEIGHT, TILE_GROUP_Y_SPACE, MAX_INPUT_DELAY);

Homepage hPage;
HomepageConfiguration hpConfig;
int main()
{
	hpConfig = ResourceManager::LoadHomepageConfiguration();
	hPage.LoadConfig(hpConfig);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_RESIZABLE, false);

	GLFWwindow* window = glfwCreateWindow(hpConfig.ScreenWidth, hpConfig.ScreenHeight, "Homepage", nullptr, nullptr);
	glfwMakeContextCurrent(window);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		OTG_LOG("Failed to initialize GLAD");

		return -1;
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	glViewport(0, 0, hpConfig.ScreenWidth, hpConfig.ScreenHeight);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	hPage.PerformPreloading();
	glfwSwapBuffers(window);

	hPage.Init();

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		
		hPage.ProcessInput(deltaTime);

		
		hPage.Update(deltaTime);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		hPage.Render();

		glfwSwapBuffers(window);
	}


	ResourceManager::Clear();

	glfwTerminate();
	return 0;

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			hPage.Keys[key] = true;
		else if (action == GLFW_RELEASE)
			hPage.Keys[key] = false;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{


	glViewport(0, 0, width, height);
}

