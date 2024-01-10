// Kyle Pointer
// SDLTest2 - heightmap generator w/ erosion
// map.cpp - Code for map class containing all tile objects
// January 2024

#include "map.h"


Map::Map() {
	this->width = 192;
	this->height = 108;
	this->erosionStrength = 0.5f;
	this->flowVariance = 0.2f;
	this->genTerrain_Random();
}
Map::Map(int W, int H, float iniErosionStrength, float iniFlowVar) {
	this->width = W;
	this->height = H;
	this->erosionStrength = iniErosionStrength;
	this->flowVariance = iniFlowVar;
	this->genTerrain_Random();
}
Map::Map(int W, int H, float iniErosionStrength, float iniFlowVar, int iniWater, int maxH, int terrainType) {
	this->width = W;
	this->height = H;
	this->erosionStrength = iniErosionStrength;
	this->flowVariance = iniFlowVar;
	if (terrainType == 0) {
		this->genTerrain_Cone(maxH);
	} else if (terrainType == 1) {
		this->genTerrain_Slope(maxH);
	} else {
		this->genTerrain_Random(maxH);
	}
	
}
Map::~Map() {
	for (std::vector<Tile> v : this->tileGrid) {  v.clear();  }
	this->tileGrid.clear();
}

Tile* Map::getTileAt(int xCoord, int yCoord) {
	if (xCoord < tileGrid.size() && xCoord >= 0) {
		if (yCoord < tileGrid[xCoord].size() && yCoord >= 0) {
			return &(this->tileGrid[xCoord][yCoord]);
		} else {
			printf("Attention! yCoord %d is invalid!", yCoord);
		}
	} else {
		printf("Attention! xCoord %d is invalid!", xCoord);
	}
	return NULL;
}

void Map::findNeighbors() {
	for (std::vector<Tile> v : this->tileGrid) {
		for (Tile t : v) {
			std::pair<int, int> coords = t.getCoords();							// store this tile's coordinates

			// Discover E neighbor:
			if ((std::get<0>(coords) + 1) < (this->width)) {
				t.setNeighbor(0, this->getTileAt(std::get<0>(coords) + 1, std::get<1>(coords)));
			}
			// Discover NE neighbor:
			if (((std::get<0>(coords) + 1) < (this->width)) && ((std::get<1>(coords)-1) >= 0)) {
				t.setNeighbor(1, this->getTileAt(std::get<0>(coords) + 1, std::get<1>(coords) - 1));
			}
			// Discover N neighbor:
			if ((std::get<1>(coords) - 1) >= 0) {
				t.setNeighbor(2, this->getTileAt(std::get<0>(coords), std::get<1>(coords) - 1));
			}
			// Discover NW neighbor:
			if (((std::get<0>(coords) - 1) >= 0) && ((std::get<1>(coords) - 1) >= 0)) {
				t.setNeighbor(3, this->getTileAt(std::get<0>(coords) - 1, std::get<1>(coords) - 1));
			}
			// Discover W neighbor:
			if ((std::get<0>(coords) - 1) >= 0) {
				t.setNeighbor(4, this->getTileAt(std::get<0>(coords) - 1, std::get<1>(coords)));
			}
			// Discover SW neighbor:
			if (((std::get<0>(coords) - 1) >= 0) && ((std::get<1>(coords) + 1) < (this->height))) {
				t.setNeighbor(5, this->getTileAt(std::get<0>(coords) + 1, std::get<1>(coords) - 1));
			}
			// Discover S neighbor:
			if ((std::get<1>(coords) + 1) < (this->height)) {
				t.setNeighbor(6, this->getTileAt(std::get<0>(coords), std::get<1>(coords) - 1));
			}
			// Discover SE neighbor:
			if (((std::get<0>(coords) + 1) < (this->width)) && (std::get<1>(coords) + 1) < (this->height)) {
				t.setNeighbor(7, this->getTileAt(std::get<0>(coords) - 1, std::get<1>(coords) - 1));
			}
		}
	}
}

void Map::dropRain(int x, int y, int n) {
	Tile* target = this->getTileAt(x, y);
	target->addWater(n);
}

void Map::randRain(int n) {
	for (int i=n; i>0; i--) {
		this->dropRain(rand() % (this->width), rand() % (this->height), 1);
	}
}

unsigned int Map::tick() {
	// TODO: tick
	return 0;
}

uint8_t Map::getSurfaceHeightAt(int xCoord, int yCoord) {
	return this->getTileAt(xCoord, yCoord)->getSurface();
}

void Map::genTerrain_Random(uint8_t maxHeight) {
	for (int i = 0; i < this->width; i++) {
		this->tileGrid.push_back(std::vector<Tile>(0));
		for (int j = 0; j < this->height; j++) {
			this->tileGrid[i].push_back(Tile(i, j, rand() % 255));
		}
	}
}

void Map::genTerrain_Cone(uint8_t maxHeight) {
	// TODO: generate cone terrain
	this->genTerrain_Random(maxHeight);
}

void Map::genTerrain_Slope(uint8_t maxHeight) {
	// TODO: generate slope terrain
	this->genTerrain_Random(maxHeight);
}

void Map::addTerrainNoise(uint8_t maxDelta) {
	// TODO: add terrain noise
}
