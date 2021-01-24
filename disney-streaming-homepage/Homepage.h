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
	float MaxInputDelay;
	unsigned int Width, Height, ResolutionWidth, ResolutionHeight,CurrentColumnSelection,CurrentRowSelection;
	std::vector<TileGroup> TileGroups;

	Homepage(unsigned int width, unsigned int height,unsigned int targetResolutionWidth,unsigned int targetResolutionHeight, float tileGroupYSpace, float maxInputDelay );
	~Homepage();
	void DrawSplashScreen();
	void PopulateTileGroups(float xScale, float yScale);
	void Init();
	void ProcessInput(float dt);
	void Update(float dt);
	void Render();

	private:
		void InitializeTileGroupPositions();
		float timeSinceLastInput;
	
};


#endif // !HOMEPAGE_H