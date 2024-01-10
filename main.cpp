// Kyle Pointer
// SDLTest2 - heightmap generator w/ erosion
// main function
// January 2024

// Library inclusions:
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <SDL.h>
#include <time.h>
#include "map.h"

// Global constants:
const int WIN_WIDTH = 1920;														// game window width
const int WIN_HEIGHT = 1080;													// game window height
const float EROSION_STRENGTH = 0.5f;											// relative strength of erosion forces
const float FLOW_VARIANCE = 0.2f;												// relative variability of water flow

// DEBUG:
const int MAX_FRAME_COUNT = 1000;												// maximum number of frames for the game to run
const int MIN_FRAME_TIME = 17;													// minimum delay between frames (in addition to processing time)


// Declare main method namespaces:
void clearScreen(SDL_Surface* targetSurf);
void drawSquare(SDL_Surface* destSurf, int xCoord, int yCoord, Uint32 colorIn);

int main(int argc, char* args[])
{
	srand(time(NULL));													// seed random number generator

	// Declare SDL variables:
	SDL_Window* window = NULL;													// game window
	SDL_Surface* screenSurface = NULL;											// game surface (on which all images will be drawn)

	// Declare asset namespaces:
	SDL_Surface* gSquare = NULL;												// basic square sprite

	// Initialize SDL:
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else {
		// Create SDL window:
		window = SDL_CreateWindow("SDLTest2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);

		if (window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else {
			// Get window surface:
			screenSurface = SDL_GetWindowSurface(window);

			// Load images, checking that loading was successful:
			gSquare = SDL_LoadBMP("img/tensq.bmp");

			if (gSquare == NULL) {
				printf("Image %s could not be loaded! Error: %s\n", "img/tensq.bmp", SDL_GetError());
			} else {
				int mapW = (int)floor(WIN_WIDTH / 10);
				int mapH = (int)floor(WIN_HEIGHT / 10);
				Map terrainMap = Map(mapW, mapH, EROSION_STRENGTH, FLOW_VARIANCE);

				// Loop for [MAX_FRAME_COUNT] frames:
				for (int i = MAX_FRAME_COUNT; i > 0; i--) {
					terrainMap.tick();

					clearScreen(screenSurface);

					/*drawSquare(screenSurface, 0, 0, 32);
					drawSquare(screenSurface, 1, 0, 64);
					drawSquare(screenSurface, 0, 1, 64);
					drawSquare(screenSurface, 1, 1, 128);
					drawSquare(screenSurface, 3, 0, 255);
					drawSquare(screenSurface, 0, 3, 255);
					drawSquare(screenSurface, 3, 3, 255);*/

					for (int i=0; i<mapW; i++) {
						for (int j=0; j<mapH; j++) {
							drawSquare(screenSurface, i, j, terrainMap.getSurfaceHeightAt(i, j));
						}
					}

					SDL_UpdateWindowSurface(window);							// update window after each draw
				}

				SDL_Delay(MIN_FRAME_TIME);									// insert minimum delay between frames (17ms <= 60fps)
			}
		}
	}

	// Free up surface assets:
	SDL_FreeSurface(gSquare);												// release asset memory
	gSquare = NULL;																// set pointers to NULL for safety
	SDL_DestroyWindow(window);													// kill SDL window
	SDL_Quit();																	// safely end SDL systems

	return 0;																	// safely exit main; return code 0 (normal exit)
}

void clearScreen(SDL_Surface* targetSurf) {
	SDL_FillRect(targetSurf, NULL, SDL_MapRGB(targetSurf->format, 0x16, 0x12, 0x00));
}

void drawSquare(SDL_Surface* destSurf, int xCoord, int yCoord, Uint32 colorIn) {
	SDL_Rect* targetRect = new SDL_Rect{ xCoord * 10, yCoord * 10, 10, 10 };
	SDL_FillRect(destSurf, targetRect, SDL_MapRGB(destSurf->format, colorIn, colorIn, colorIn));
}