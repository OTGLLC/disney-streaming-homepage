#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <vector>
#include "TileGroup.h"
enum HomepageState
{
	HOMEPAGE_LOADING,
	HOMEPAGE_ACTIVE
};

class Homepage
{
public:
	float TileGroupYSpace;
	HomepageState State;
	bool Keys[1024];
	unsigned int Width, Height, ResolutionWidth, ResolutionHeight;
	std::vector<TileGroup> TileGroups;

	Homepage(unsigned int width, unsigned int height,unsigned int targetResolutionWidth,unsigned int targetResolutionHeight, float tileGroupYSpace );
	~Homepage();
	void DrawSplashScreen();
	void PopulateTileGroups(float xScale, float yScale);
	void Init();
	void ProcessInput(float dt);
	void Update(float dt);
	void Render();

	private:
		void InitializeTileGroupPositions();
	
};


#endif // !HOMEPAGE_H