// COMP710 GP Framework 2024

//// Library includes:
//#include <SDL.h>
#include <time.h>
#include <crtdbg.h>

// Local includes:
#include "game.h"
#include "logmanager.h"
#include <iostream>
#include "glew.h"
int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	srand(time(nullptr));
	Game& gameInstance = Game::GetInstance();
	if (!gameInstance.Initialize())
	{
		LogManager::GetInstance().Log("Game initialize failed!");

		return 1;
	}

	while (gameInstance.DoGameLoop())
	{
		// No body.
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			std::cerr << "OpenGL Error: " << error << std::endl;
		}
	}

	Game::DestroyInstance();
	LogManager::DestroyInstance();

	return 0;
}