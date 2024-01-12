// Kyle Pointer
// SDLTest2 - heightmap generator w/ erosion
// map.h - header for map object containing all tile objects
// January 2024

#pragma once
#ifndef MAP
#define MAP

#include <vector>
#include <SDL.h>
#include "tile.h"

class Map {																// Map class: simulation manager and container for tile objects
public :
	Map();																		// default constructor
	//Map(int W, int H, float iniErosionStrength, float iniFlowVar);				// constructor with dims, erosion strength, and flow variance parameters
	Map(int W, int H, float iniErosionStrength, int iniWater, int maxH, int terrainType, int maxNoise);
	~Map();																		// destructor
	Tile* getTileAt(int xCoord, int yCoord);									// fetches pointer to tile located at specified coords
																					// returns: pointer to specified Tile
	void findNeighbors();														// iterates through each tile, finding and assigning neighbors
	void dropRain(int x, int y, int n);											// adds n units of water onto tile at specified x,y coords
	void randRain(int n);														// adds n units of water onto random tiles
	void tick();																// performs 1 tick of water flow simulation
	void tickN(int n);															// performs n ticks of water flow simulation
	uint8_t getSurfaceHeightAt(int xCoord, int yCoord);							// fetches the surface height of the tile at the specified coords
																					// returns: specified tile's surface height (0-255 inclusive)

private :
	int width;																	// map width, in tiles
	int height;																	// map height, in tiles
	int seaLevel;																// elevation below which terrain is covered by water
	std::vector<std::vector<Tile>> tileGrid;									// 2d vector of Tile objects
	float erosionStrength;														// decimal (0-1 inclusive) prescribing relative erosion strength
	void genTerrain_Random(uint8_t maxHeight, int noiseFactor);					// generate random terrain, up to specified max height (default=255)
	void genTerrain_Cliff(uint8_t maxHeight, int noiseFactor);					// generate terrain with a cliff bisecting the map
	void genTerrain_Cone(uint8_t maxHeight, int noiseFactor);					// generate terrain in a cone shape, up to specified max (default=255)
	void genTerrain_Slope(uint8_t maxHeight, int noiseFactor);					// generate terrain in a North-to-South slope, to max (default=255)
	void addTerrainNoise(uint8_t maxDelta);										// add noise to existing terrain, up to a specified change
};
#endif
