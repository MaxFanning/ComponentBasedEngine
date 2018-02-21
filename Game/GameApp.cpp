#include "GameApp.h"

GameApp::GameApp()
{
}

GameApp::~GameApp()
{

}

void GameApp::OnInitialize(u32 width, u32 height)
{
	// Create our window and attach to our application
	mWindow.Initialize(GetInstance(), GetAppName(), width, height);
	HookWindow(mWindow.GetWindowHandle());

	// Initialize our timer
	mTimer.Initialize();

	// Initialice the graphics systems
	GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);
	MeshRenderer::StaticInitialize();
	SpriteRenderer::StaticInitialize();
	ModelManager::StaticInitialize(nullptr);
	TextureManager::StaticInitialize(nullptr);
	SimpleDraw::Initialize(999999);

	// Initialize the input system
	InputSystem::StaticInitialize(mWindow.GetWindowHandle());

	mWorld.Intialize(1000);
	mWorld.LoadLevel("../Data/Levels/test_level.xml"); // Replaces 'World.Create(all of the different xmls..)'

	mTerrain.Initialize("../Data/Heightmaps/heightmap.raw", 256, 256, 30.0f);
	mTerrain.SetLayer("../Data/Images/mud.png", 0, 0.0f, 20.0f, 17.0f);
	mTerrain.SetLayer("../Data/Images/rock.jpg", 1, 15.0f, 30.0f, 19.0f);
	mTexture.Initialize("../Data/Images/target.png");

	// Pre-Load all of the models
	ModelManager::Get()->Load("../Data/Models/duck.txt");
	ModelManager::Get()->Load("../Data/Models/jeep.txt");
	ModelManager::Get()->Load("../Data/Models/snorlax.txt");
}

void GameApp::OnTerminate()
{
	mTexture.Terminate();
	mTerrain.Terminate();

	mWorld.Terminate();

	// Terminate our sub-systems
	InputSystem::StaticTerminate();
	SimpleDraw::Terminate();
	TextureManager::StaticTerminate();
	ModelManager::StaticTerminate();
	SpriteRenderer::StaticTerminate();
	MeshRenderer::StaticTerminate();
	GraphicsSystem::StaticTerminate();

	// Detach our window and terminate it
	UnhookWindow();
	mWindow.Terminate();
}

void GameApp::OnUpdate()
{
	// Handle message from the operating system
	if (mWindow.HandleMessage())
	{
		// We received a quit signal, just exit
		mRunning = false;
		return;
	}

	InputSystem* inputSystem = InputSystem::Get();
	GraphicsSystem* graphicsSystem = GraphicsSystem::Get();

	// Listen for inputs and update
	inputSystem->Update();
	mTimer.Update();

	//Update world
	mWorld.Update(mTimer.GetElapsedTime());

	// Check input
	if (inputSystem->IsKeyPressed(Keys::ESCAPE))
	{
		// If ESC is pressed, we are done
		PostQuitMessage(0);
		return;
	}

	graphicsSystem->BeginRender(Color::Black());

	// Render World
	MeshRenderer::Get()->SetCamera(*CameraComponent::main);
	mWorld.Render();

	mTerrain.Render();
	if (inputSystem->IsKeyDown(Keys::SPACE))
	{
		mTerrain.DebugRender();
	}

	// Render World 2D
	SpriteRenderer::Get()->BeginRender();
	mWorld.Render2D();
	//SpriteRenderer::Get()->Draw(mTexture,
	//	Math::Vector2((float)inputSystem->GetMouseScreenX(), (float)inputSystem->GetMouseScreenY()));
	SpriteRenderer::Get()->Draw(mTexture,
		Math::Vector2(1024 / 2 - 31, 768 / 2 - 31));
	SpriteRenderer::Get()->EndRender();

	SimpleDraw::AddLine(Math::Vector3::Zero(), Math::Vector3::XAxis(), Color::Red());
	SimpleDraw::AddLine(Math::Vector3::Zero(), Math::Vector3::YAxis(), Color::Green());
	SimpleDraw::AddLine(Math::Vector3::Zero(), Math::Vector3::ZAxis(), Color::Blue());

	SimpleDraw::Render(*CameraComponent::main);

	graphicsSystem->EndRender();
}