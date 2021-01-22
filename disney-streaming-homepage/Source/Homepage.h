#ifndef HOMEPAGE_H
#define HOMEPAGE_H

enum HomepageState
{
	PAGE_LOAD,
	PAGE_ACTIVE
};

class Homepage
{
public:
	HomepageState State;
	bool Keys[1024];
	unsigned int Width, Height;

	Homepage(unsigned int width, unsigned int height);
	~Homepage();
	void Init();
	void ProcessInput(float dt);
	void Update(float dt);
	void Render();
};


#endif // !HOMEPAGE_H