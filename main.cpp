#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "MNL.h"

//Set window size
const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 272;

bool init();
void close();

SDL_Window*		gWindow = nullptr;

bool init()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Log("SDL could not initialized. SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			SDL_Log("Warning: Linear texture filtering disalbed\n");
		}

		//Create window
		gWindow = SDL_CreateWindow("PuzzleToSurvive",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN
		);
		if (nullptr == gWindow)
		{
			SDL_Log("Window could not be created. SDL_Error: %s\n", SDL_GetError());
			return false;
		}
		else
		{
			//Initialize renderer for GPU accelerate
			SDL_Renderer* pRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (nullptr == pRenderer)
			{
				SDL_Log("Renderer could not be created. SDL_Error: %s\n", SDL_GetError());
			}
			else
			{
				SDL_SetRenderDrawColor(pRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				//Flag for using PNG
				int imgFlags = IMG_INIT_PNG;
				//Initialize SDL_image
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					SDL_Log("SDL_image initializing failure. SDL_image Error: %s\n", IMG_GetError());
					return false;
				}
				else
				{
					MNL_INIT(pRenderer);
				}
			}
		}
	}

	return true;
}

void close()
{
	//Close application
	SDL_DestroyRenderer(MNCore::GetRenderer());
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;

	MNL_QUIT();
	IMG_Quit();
	SDL_Quit();
}


int main(int argc, char* args[])
{

	if (!init())
	{
		SDL_Log("Initializing failure\n");
	}
	else
	{
		bool quit = false;
		SDL_Event e;

		bool buttonDown = false;
		//Enter game loop
		while (!quit)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				//Handle events
				if (e.type == SDL_QUIT) quit = true;
				if (e.type == SDL_MOUSEBUTTONDOWN) buttonDown = true;
				else if (SDL_MOUSEBUTTONUP) buttonDown = false;
			}
			//Clear screen
			SDL_Renderer* pRenderer = MNCore::GetRenderer();
			SDL_SetRenderDrawColor(pRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(pRenderer);

			/* Game updates here */

			if (buttonDown)
				SDL_Log("down");

			//Swap screen buffer
			SDL_RenderPresent(pRenderer);
		}
	}
	//Closing
	close();
	return 0;
}