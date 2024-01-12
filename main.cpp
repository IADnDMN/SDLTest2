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

// Global parameters:
const int WIN_WIDTH = 1900;														// game window width
const int WIN_HEIGHT = 1000;													// game window height
const int INITIAL_RAIN = 1000;													// units of rain to begin on the map
const int RAIN_PER_TICK = 100;													// units of rain to fall each tick of the simulation
const float EROSION_STRENGTH = 0.6f;											// relative strength of erosion forces
const float FLOW_VARIANCE = 0.2f;												// relative variability of water flow
const int MAP_WIDTH = 190;														// width of the map, in tiles
const int MAP_HEIGHT = 100;														// height of the map, in tiles
const int MAX_MAP_NOISE = 30;
const int MAX_FRAME_COUNT = 1000;												// maximum number of frames for the sim to run

const bool DRAW_WATER = false;													// DEBUG: whether to draw tiles as blue if underwater
const int POSTSIM_PAUSE = 5000;													// DEBUG: time (ms) to hold the final frame before exiting


// Declare main method namespaces:
void clearScreen(SDL_Surface* targetSurf);
void drawSquare(SDL_Surface* destSurf, int xCoord, int yCoord, Uint32 colorIn);
void drawMap(Map* map, SDL_Surface*);

int main(int argc, char* args[])
{
	srand(time(NULL));													// seed random number generator

	// Declare SDL variables:
	SDL_Window* window = NULL;													// game window
	SDL_Surface* screenSurface = NULL;											// game surface (on which all images will be drawn)

	// Initialize SDL:
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return 1;																// RETURN 1: SDL failed to initialize
	}
	else {
		// Create SDL window:
		window = SDL_CreateWindow("SDLTest2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);

		if (window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return 2;															// RETURN 2: SDL window could not be created
		}
		else {
			// Get window surface:
			screenSurface = SDL_GetWindowSurface(window);

			Map terrainMap = Map(MAP_WIDTH, MAP_HEIGHT, EROSION_STRENGTH, FLOW_VARIANCE, INITIAL_RAIN, 255, 1, MAX_MAP_NOISE);

			// Loop for [MAX_FRAME_COUNT] frames:
			for (int i=MAX_FRAME_COUNT; i>0; i--) {
				terrainMap.tick();
				terrainMap.randRain(RAIN_PER_TICK);
				drawMap(&terrainMap, screenSurface);

				SDL_UpdateWindowSurface(window);								// update window after each draw
			}
		}
	}

	SDL_Delay(POSTSIM_PAUSE);
	SDL_DestroyWindow(window);													// kill SDL window
	SDL_Quit();																	// safely end SDL systems

	return 0;																	// safely exit main; return code 0 (normal exit)
}

void clearScreen(SDL_Surface* targetSurf) {
	SDL_FillRect(targetSurf, NULL, SDL_MapRGB(targetSurf->format, 0x0, 0x00, 0x10));
}

void drawSquare(SDL_Surface* destSurf, int xCoord, int yCoord, Uint8 elevation) {
	SDL_Rect* targetRect = new SDL_Rect{ xCoord * 10, yCoord * 10, 10, 10 };
	SDL_FillRect(destSurf, targetRect, SDL_MapRGB(destSurf->format, elevation, elevation, elevation));
	delete targetRect;
}

void drawWater(SDL_Surface* destSurf, int xCoord, int yCoord, Uint8 elevation, Uint8 depth) {
	SDL_Rect* targetRect = new SDL_Rect{ xCoord * 10, yCoord * 10, 10, 10 };
	SDL_FillRect(destSurf, targetRect, SDL_MapRGB(destSurf->format, elevation, elevation, 255 - depth));
	delete targetRect;
}

void drawMap(Map* map, SDL_Surface* screen) {
	clearScreen(screen);
	for (int i = 0; i<MAP_WIDTH; i++) {
		for (int j = 0; j<MAP_HEIGHT; j++) {
			if (DRAW_WATER && map->getTileAt(i, j)->getWaterDepth() > 0) {
				drawWater(screen, i, j, map->getSurfaceHeightAt(i, j), map->getTileAt(i, j)->getWaterDepth());
			} else {
				drawSquare(screen, i, j, map->getTileAt(i, j)->getElevation());
			}
		}
	}
}
