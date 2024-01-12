// Kyle Pointer
// SDLTest2 - heightmap generator w/ erosion
// tile.h - header for tile object containing specific terrain info
// January 2024

#pragma once
#ifndef TILE
#define TILE


class Tile {															// Tile class: fundamental spatial unit of the simulation
public :
	Tile();																		// default constructor
	Tile(int xCoord, int yCoord, uint8_t elevation, float erosion);				// constructor with x,y coordinates, and elevation parameters
	~Tile();																	// destructor
	std::pair<int,int> getCoords();												// returns a pair container with X and Y coordinates of this tile
	void setNeighborByDir(int dir, Tile* target);								// sets neighbor pointer in specified direction (0=East, CCW to 7=Southeast)
	Tile* getNeighborByDir(int dir);											// gets neighbor pointer in specified direction (0=East, CCW to 7=Southeast)
	uint8_t getElevation();														// returns height of this tile's terrain, excluding any water
	uint8_t getWaterDepth();													// returns depth of water on this tile
	uint8_t getSurface();														// calculates the sum of the tile's elevation and waterDepth; returns result
	uint8_t addSoil(uint8_t amount);											// adds an amount to this tile's elevation; returns new elevation
	uint8_t remSoil(uint8_t amount);											// removes an amount of soil from this tile's elevation; returns new elevation
	uint8_t addWater(uint8_t amount);											// adds an amount of water to this tile's waterDepth; returns new depth
	uint8_t remWater(uint8_t amount);											// removes an amount of water from this tile's waterDepth; returns new depth
	void settleWater();															// attempts to correct discrepancies in water surface height with neighbors

private :
	int X;																		// x-coordinate in parent map object
	int Y;																		// y-coordinate in parent map object
	float erosionStrength;														// erosion strength, received from Map class
	uint8_t elevation;															// elevation value (0-255 inclusive)
	uint8_t waterDepth;															// depth of standing water on this tile
	// pointers to neighboring tiles:
	Tile* neighbor_E;
	Tile* neighbor_NE;
	Tile* neighbor_N;
	Tile* neighbor_NW;
	Tile* neighbor_W;
	Tile* neighbor_SW;
	Tile* neighbor_S;
	Tile* neighbor_SE;
	int getSlope(uint8_t dir);													// calculate and return slope value for the specified direction
	int getWaterSlope(uint8_t dir);												// 
	uint8_t giveSoil(Tile* recipient, uint8_t amount);							// gives some elevation from itself to specified tile; returns new elevation
	uint8_t giveWater(Tile* recipient, uint8_t amount = 1);						// gives some water from itself to specified tile; returns new waterDepth
	bool erosionCheck();														// tests whether to erode based on 
};
#endif
