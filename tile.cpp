// Kyle Pointer
// SDLTest2 - heightmap generator w/ erosion
// tile.cpp - Code for tile class defining each spatial unit of the map
// January 2024

#include <utility>
#include <string>
#include "tile.h"

const int PREVENT_FLOW_OFFMAP = 0;


Tile::Tile() {
	this->X = 0;
	this->Y = 0;
	this->erosionStrength = 0.0f;
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
}
Tile::Tile(int xCoord, int yCoord, uint8_t elevation, float erosion) {
	this->X = xCoord;
	this->Y = yCoord;
	this->erosionStrength = erosion;
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
}
Tile::~Tile() {
	// Nothing to delete!
}

std::pair<int,int> Tile::getCoords() {
	return std::pair<int, int>{this->X, this->Y};
}

void Tile::setNeighborByDir(int dir, Tile* target) {
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
			printf("Invalid direction %d given to setNeighborByDir!\n", dir);
			break;
	}
}

Tile* Tile::getNeighborByDir(int dir) {
	switch (dir) {
	case 0:
		return this->neighbor_E;
	case 1:
		return this->neighbor_NE;
	case 2:
		return this->neighbor_N;
	case 3:
		return this->neighbor_NW;
	case 4:
		return this->neighbor_W;
	case 5:
		return this->neighbor_SW;
	case 6:
		return this->neighbor_S;
	case 7:
		return this->neighbor_SE;
	default:
		printf("Invalid direction %d given to getNeighborByDir!\n", dir);
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
	//return (uint8_t)fmin(255, (this->elevation + this->waterDepth));
	int e = this->elevation;
	int w = this->waterDepth;
	return (uint8_t)fmin(255, (e + w));
}

uint8_t Tile::addSoil(uint8_t amount) {
	if (255 - this->elevation < amount) {
		this->elevation = 255;
	} else {
		this->elevation += amount;
	}
	return this->elevation;
}

uint8_t Tile::remSoil(uint8_t amount) {
	if (this->elevation <= amount) {
		this->elevation = 0;
	} else {
		this->elevation -= amount;
	}
	return this->elevation;
}

uint8_t Tile::addWater(uint8_t amount) {
	if (255 - this->getSurface() < amount) {
		this->addWater(255 - this->getSurface());
	} else {
		this->waterDepth += amount;
	}
	return this->waterDepth;
}

uint8_t Tile::remWater(uint8_t amount) {
	if (this->waterDepth <= amount) {
		this->waterDepth = 0;
	}
	else {
		this->waterDepth -= amount;
	}
	return this->waterDepth;
}

void Tile::settleWater() {
	if (this->waterDepth > 0) {
		//Tile* target = NULL;
		int flowPool = 0;
		int deltas[8] = { 0,0,0,0,0,0,0,0 };
		int d = 0;

		for (int i = 0; i < 8; i++) {
			d = this->getWaterSlope(i);
			if (d <= 0) {
				deltas[i] = 1 + abs(d);
				flowPool += 1 + abs(d);
			}
		}
		if (flowPool > 0) {
			int r = rand() % flowPool;
			int i = 0;
			while (r > 0 && i < 7) {
				r -= deltas[i];
				i++;
			}
			Tile* target = this->getNeighborByDir(i);
			this->giveWater(target, 1);
			if (this->getSlope(i) <= 0) {
				if (this->erosionCheck()) {
					this->giveSoil(target, 1);
				}
			}
		}
	}
}

int Tile::getSlope(uint8_t dir) {
	switch (dir) {
		case 0:
			if (this->neighbor_E == NULL) { return 0 - this->elevation; }
			else { return (int)this->neighbor_E->getElevation() - (int)this->elevation; }
		case 1:
			if (this->neighbor_NE == NULL) { return 0 - this->elevation; }
			else { return (int)this->neighbor_NE->getElevation() - (int)this->elevation; }
		case 2:
			if (this->neighbor_N == NULL) { return 0 - this->elevation; }
			else { return (int)this->neighbor_N->getElevation() - (int)this->elevation; }
		case 3:
			if (this->neighbor_NW == NULL) { return 0 - this->elevation; }
			else { return (int)this->neighbor_NW->getElevation() - (int)this->elevation; }
		case 4:
			if (this->neighbor_W == NULL) { return 0 - this->elevation; }
			else { return (int)this->neighbor_W->getElevation() - (int)this->elevation; }
		case 5:
			if (this->neighbor_SW == NULL) { return 0 - this->elevation; }
			else { return (int)this->neighbor_SW->getElevation() - (int)this->elevation; }
		case 6:
			if (this->neighbor_S == NULL) { return 0 - this->elevation; }
			else { return (int)this->neighbor_S->getElevation() - (int)this->elevation; }
		case 7:
			if (this->neighbor_SE == NULL) { return 0 - this->elevation; }
			else { return (int)this->neighbor_SE->getElevation() - (int)this->elevation; }
		default:
			printf("Error! Invalid dir %d passed to getSlope()!\n", dir);
			return 0;
	}
}

int Tile::getWaterSlope(uint8_t dir) {
	int surfaceHeight = getSurface();
	switch (dir) {
	case 0:
		if (this->neighbor_E == NULL) { return PREVENT_FLOW_OFFMAP; }
		else { return (int)this->neighbor_E->getSurface() - surfaceHeight; }
	case 1:
		if (this->neighbor_NE == NULL) { return PREVENT_FLOW_OFFMAP; }
		else { return (int)this->neighbor_NE->getSurface() - surfaceHeight; }
	case 2:
		if (this->neighbor_N == NULL) { return PREVENT_FLOW_OFFMAP; }
		else { return (int)this->neighbor_N->getSurface() - surfaceHeight; }
	case 3:
		if (this->neighbor_NW == NULL) { return PREVENT_FLOW_OFFMAP; }
		else { return (int)this->neighbor_NW->getSurface() - surfaceHeight; }
	case 4:
		if (this->neighbor_W == NULL) { return PREVENT_FLOW_OFFMAP; }
		else { return (int)this->neighbor_W->getSurface() - surfaceHeight; }
	case 5:
		if (this->neighbor_SW == NULL) { return PREVENT_FLOW_OFFMAP; }
		else { return (int)this->neighbor_SW->getSurface() - surfaceHeight; }
	case 6:
		if (this->neighbor_S == NULL) { return PREVENT_FLOW_OFFMAP; }
		else { return (int)this->neighbor_S->getSurface() - surfaceHeight; }
	case 7:
		if (this->neighbor_SE == NULL) { return PREVENT_FLOW_OFFMAP; }
		else { return (int)this->neighbor_SE->getSurface() - surfaceHeight; }
	default:
		printf("Error! Invalid dir %d passed to getSlope()!\n", dir);
		return 0;
	}
}

uint8_t Tile::giveSoil(Tile* recipient, uint8_t amount) {
	if (this->elevation > 0) {
		if (recipient != NULL) {
			recipient->addSoil(amount);
		}
		this->remSoil(amount);
	}
	return this->elevation;
}

uint8_t Tile::giveWater(Tile* recipient, uint8_t amount) {
	if (this->waterDepth > 0) {
		if (recipient != NULL) {
			recipient->addWater(amount);
		}
		this->remWater(amount);
	}
	return this->waterDepth;
}

bool Tile::erosionCheck() {
	return (rand() % 1000 < (this->erosionStrength * 1000)) ? true : false;
}
