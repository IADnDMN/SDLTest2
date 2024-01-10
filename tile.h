// Kyle Pointer
// SDLTest2 - heightmap generator w/ erosion
// tile.h - header for tile object containing specific terrain info
// January 2024

#pragma once
#ifndef TILE
#define TILE


class Tile {															// Tile class: basic spatial unit of the simulation, to populate Map object
public :
	Tile();																		// default constructor
	Tile(int xCoord, int yCoord, uint8_t elevation);							// constructor with x,y coordinates, and elevation parameters
	~Tile();																	// destructor
	std::pair<int,int> getCoords();											// fetches X and Y coordinates of this tile
																					// returns: pair container with elements X and Y
	void setNeighbor(int dir, Tile* target);									// sets neighbor pointer in specified direction (0=East, CCW to 7=Southeast)
	uint8_t getElevation();														// fetches height of terrain, excluding any water
																					// returns: this->elevation
	uint8_t getWaterDepth();													// fetches depth of static water on this tile
																					// returns: this->waterDepth
	uint8_t getSurface();														// calculates and reports the sum of the tile's elevation and waterDepth
																					// returns: (this->elevation + this->waterDepth)
	uint8_t addSoil(uint8_t amount = 1);										// adds an amount (default=1) to this tile's elevation
																					// returns: resulting this->elevation value
	uint8_t addWater(uint8_t amount = 1);										// adds an amount (default=1) to this tile's waterDepth
																					// returns: resulting this->waterDepth value

private :
	int X;																		// x-coordinate in parent map object
	int Y;																		// y-coordinate in parent map object
	unsigned int xpos;															// x position at which to be rendered on the screen
	unsigned int ypos;															// y position at which to be rendered on the screen
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
	// E->W, NE->SW, N->S, and NW->SE slope measurements, respectively:
	float slope_EW;
	float slope_NESW;
	float slope_NS;
	float slope_NWSE;
	void calcSlopes();															// calculate all slope values for this tile
	uint8_t giveSoil(Tile* recipient, uint8_t amount=1);						// gives an amount (default=1) from itself to specified tile's elevation
																					// returns: resulting this.elevation value
	uint8_t giveWater(Tile* recipient, uint8_t amount = 1);						// gives an amount (default=1) from itself to another tile's waterDepth
																					// returns: resulting this.waterDepth value
	uint8_t settleWater();														// attempts to correct discrepancies in water surface height with neighbors
																					// returns: resulting this.waterDepth value
};
#endif
