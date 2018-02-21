#ifndef INCLUDED_GAME_H
#define INCLUDED_GAME_H

#include <Engine.h>

class GameApp : public Application
{
public:
	GameApp();
	~GameApp();

private:
	void OnInitialize(u32 width, u32 height) override;
	void OnTerminate() override;
	void OnUpdate() override;

	Window mWindow;
	Timer mTimer;

	World mWorld;

	Terrain mTerrain;
	Texture mTexture;
};

#endif