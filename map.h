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

class Map : public Tile {												// Map class extends Tile class to gain access to protected methods
public :
	Map();																		// default constructor
	Map(int W, int H, float iniErosionStrength, float iniFlowVar);				// constructor with dims, erosion strength, and flow variance parameters
	Map(int W, int H, float iniErosionStrength, float iniFlowVar, int iniWater, int maxH=255, int terrainType=-1);
	~Map();																		// destructor
	Tile* getTileAt(int xCoord, int yCoord);									// fetches pointer to tile located at specified coords
																					// returns: pointer to specified Tile
	void findNeighbors();														// iterates through each tile, finding and assigning neighbors
	void dropRain(int x, int y, int n);											// adds n units of water onto tile at specified x,y coords
	void randRain(int n);														// adds n units of water onto random tiles
	unsigned int tick();														// performs 1 tick of water flow simulation
																					// returns: number of water transfers between tiles
	uint8_t getSurfaceHeightAt(int xCoord, int yCoord);							// fetches the surface height of the tile at the specified coords
																					// returns: specified tile's surface height (0-255 inclusive)

private :
	int width;																	// map width, in tiles
	int height;																	// map height, in tiles
	int seaLevel;																// elevation below which terrain is covered by water
	std::vector<std::vector<Tile>> tileGrid;									// 2d vector of Tile objects
	float erosionStrength;														// decimal (0-1 inclusive) prescribing relative erosion strength
	float flowVariance;															// decimal (0-1 inclusive) prescribing relative flow direction variance
	void genTerrain_Random(uint8_t maxHeight=255);								// generate random terrain, up to specified max height (default=255)
	void genTerrain_Cone(uint8_t maxHeight=255);								// generate terrain in a cone shape, up to specified max (default=255)
	void genTerrain_Slope(uint8_t maxHeight=255);								// generate terrain in a North-to-South slope, to max (default=255)
	void addTerrainNoise(uint8_t maxDelta);										// add noise to existing terrain, up to a specified change
};
#endif