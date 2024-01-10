// Kyle Pointer
// SDLTest2 - heightmap generator w/ erosion
// tile.cpp - Code for tile class defining each spatial unit of the map
// January 2024

#include <utility>
#include <string>
#include "tile.h"


Tile::Tile() {
	this->X = 0;
	this->Y = 0;
	this->elevation = 0;
	this->waterDepth = 0;
	this->neighbor_E = NULL;
	this->neighbor_NE = NULL;
	this->neighbor_N = NULL;
	this->neighbor_NW = NULL;
	this->neighbor_W = NULL;
	this->neighbor_SW = NULL;
	this->neighbor_S = NULL;
	this->neighbor_SE = NULL;
	this->slope_EW = 0.0f;
	this->slope_NESW = 0.0f;
	this->slope_NS = 0.0f;
	this->slope_NWSE = 0.0f;
}
Tile::Tile(int xCoord, int yCoord, uint8_t elevation) {
	this->X = xCoord;
	this->Y = yCoord;
	this->elevation = elevation;
	this->waterDepth = 0;
	this->neighbor_E = NULL;
	this->neighbor_NE = NULL;
	this->neighbor_N = NULL;
	this->neighbor_NW = NULL;
	this->neighbor_W = NULL;
	this->neighbor_SW = NULL;
	this->neighbor_S = NULL;
	this->neighbor_SE = NULL;
	this->slope_EW = 0.0f;
	this->slope_NESW = 0.0f;
	this->slope_NS = 0.0f;
	this->slope_NWSE = 0.0f;
}
Tile::~Tile() {
	// Nothing to delete!
}

std::pair<int,int> Tile::getCoords() {
	return std::pair<int, int>{this->X, this->Y};
}

void Tile::setNeighbor(int dir, Tile* target) {
	switch (dir) {
		case 0:
			this->neighbor_E = target;
			break;
		case 1:
			this->neighbor_NE = target;
			break;
		case 2:
			this->neighbor_N = target;
			break;
		case 3:
			this->neighbor_NW = target;
			break;
		case 4:
			this->neighbor_W = target;
			break;
		case 5:
			this->neighbor_SW = target;
			break;
		case 6:
			this->neighbor_S = target;
			break;
		case 7:
			this->neighbor_SE = target;
			break;
		default:
			printf("Invalid direction %d given!\n", dir);
			break;
	}
}

uint8_t Tile::getElevation() {
	return this->elevation;
}

uint8_t Tile::getWaterDepth() {
	return this->waterDepth;
}

uint8_t Tile::getSurface() {
	return (uint8_t)fmin(255, (this->elevation + this->waterDepth));
}

uint8_t Tile::addSoil(uint8_t amount) {
	this->elevation = (uint8_t)fmin(255, this->elevation + amount);
	return this->elevation;
}

uint8_t Tile::addWater(uint8_t amount) {
	this->waterDepth += amount;
	return this->waterDepth;
}

void Tile::calcSlopes() {
	// TODO: calculate slopes
}

uint8_t Tile::giveSoil(Tile* recipient, uint8_t amount) {
	if (this->elevation > 0) {
		recipient->addSoil(amount);
		this->elevation = (uint8_t)fmax(0, this->elevation - amount);
	}
	return this->elevation;
}

uint8_t Tile::giveWater(Tile* recipient, uint8_t amount) {
	if (this->waterDepth > 0) {
		recipient->addWater(amount);
		this->waterDepth = (uint8_t)fmax(0, this->waterDepth - 1);
	}
	return this->waterDepth;
}

uint8_t Tile::settleWater() {
	// TODO: settle water
	return this->waterDepth;
}
