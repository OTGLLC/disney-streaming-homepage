#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <vector>
#include "TileGroup.h"
#include "ResourceManager.h"
#include "HomepageConfiguration.h"

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
	float TextYPositionOffset;
	unsigned int Width, Height, ResolutionWidth, ResolutionHeight,CurrentColumnSelection,CurrentRowSelection,ResolutionDefaultFontSize;
	std::vector<TileGroup> TileGroups;
	
	~Homepage();
	void LoadConfig(const HomepageConfiguration& config);
	void PerformPreloading();
	void PopulateTileGroups(float xScale, float yScale);
	void Init();
	void ProcessInput(float dt);
	void Update(float dt);
	void Render();


	private:
		void InitializeTileGroupPositions(float xScale, float yScale);
		float timeSinceLastInput;
		float _displayXscale;
		float _displayYscale;
	
};


#endif // !HOMEPAGE_H