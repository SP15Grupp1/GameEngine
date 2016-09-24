#include "stdafx.h"
#include "Engine\Engine.h"
#include "Engine\Rendering\DXRenderer.h"
#include "Engine\FileWatcher\FileChangeWatcher.h"

Engine * Engine::ourInstance = nullptr;

Engine::Engine()
{
	myRenderer = std::make_unique<DXRenderer>();
	myFileWatcher = new FileChangeWatcher();
	myResourceManager = std::make_unique<ResourceManager>();
	myDebugLogger = std::make_unique<DebugLogger>();
}

Engine::~Engine()
{
}

void Engine::CreateInstance()
{
	assert("Instance already created" && ourInstance == nullptr);

	ourInstance = new Engine();
}

void Engine::DestroyInstance()
{
	assert("Instance not created" && ourInstance != nullptr);

	delete ourInstance;
	ourInstance = nullptr;
}
