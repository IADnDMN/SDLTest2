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
	this->genTerrain_Random(255, 10);
	this->findNeighbors();
}
//Map::Map(int W, int H, float iniErosionStrength, float iniFlowVar) {
//	this->width = W;
//	this->height = H;
//	this->erosionStrength = iniErosionStrength;
//	this->flowVariance = iniFlowVar;
//	this->genTerrain_Random(255, RAND_NOISE);
//	this->findNeighbors();
//}
Map::Map(int W, int H, float iniErosionStrength, float iniFlowVar, int iniWater, int maxH, int terrainType, int maxNoise) {
	this->width = W;
	this->height = H;
	this->erosionStrength = iniErosionStrength;
	this->flowVariance = iniFlowVar;
	if (terrainType == 0) {
		this->genTerrain_Cliff(maxH, maxNoise);
	} else if (terrainType == 1) {
		this->genTerrain_Cone(maxH, maxNoise);
	} else if (terrainType == 2) {
		this->genTerrain_Slope(maxH, maxNoise);
	} else {
		this->genTerrain_Random(maxH, maxNoise);
	}
	this->findNeighbors();
	this->randRain(iniWater);
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
			printf("Attention! getTileAt() (%d,%d) is off the map!\n", xCoord, yCoord);
		}
	} else {
		printf("Attention! getTileAt() (%d,%d) is off the map!\n", xCoord, yCoord);
	}
	return NULL;
}

void Map::findNeighbors() {
	for (int i=0; i<this->width; i++) {
		for (int j=0; j<this->height; j++) {
			// Discover E neighbor:
			if (i + 1 < (this->width)) {
				this->tileGrid[i][j].setNeighborByDir(0, this->getTileAt(i + 1, j));
			}
			// Discover NE neighbor:
			if (((i + 1) < (this->width)) && ((j - 1) >= 0)) {
				this->tileGrid[i][j].setNeighborByDir(1, this->getTileAt(i + 1, j - 1));
			}
			// Discover N neighbor:
			if ((j - 1) >= 0) {
				this->tileGrid[i][j].setNeighborByDir(2, this->getTileAt(i, j - 1));
			}
			// Discover NW neighbor:
			if (((i - 1) >= 0) && ((j - 1) >= 0)) {
				this->tileGrid[i][j].setNeighborByDir(3, this->getTileAt(i - 1, j - 1));
			}
			// Discover W neighbor:
			if ((i - 1) >= 0) {
				this->tileGrid[i][j].setNeighborByDir(4, this->getTileAt(i - 1, j));
			}
			// Discover SW neighbor:
			if (((i - 1) >= 0) && ((j + 1) < (this->height))) {
				this->tileGrid[i][j].setNeighborByDir(5, this->getTileAt(i - 1, j + 1));
			}
			// Discover S neighbor:
			if ((j + 1) < (this->height)) {
				this->tileGrid[i][j].setNeighborByDir(6, this->getTileAt(i, j + 1));
			}
			// Discover SE neighbor:
			if (((i + 1) < (this->width)) && (j + 1) < (this->height)) {
				this->tileGrid[i][j].setNeighborByDir(7, this->getTileAt(i + 1, j + 1));
			}
		}
	}
}

void Map::dropRain(int x, int y, int n) {
	Tile* target = this->getTileAt(x, y);
	if (target == NULL) {
		printf("Invalid target set by dropRain() at (%d,%d)!", x, y);
	}
	else {
		target->addWater(n);
	}
}

void Map::randRain(int n) {
	for (int i=n; i>0; i--) {
		this->dropRain(rand() % (this->width), rand() % (this->height), 1);
	}
}

unsigned int Map::tick() {
	for (int z = 0; z < 256; z++) {
		for (int i = 0; i < this->width; i++) {
			for (int j = 0; j < this->height; j++) {
				if (this->tileGrid[i][j].getSurface() == z) {
					this->tileGrid[i][j].settleWater();
				}
			}
		}
	}
	return 0;
}

uint8_t Map::getSurfaceHeightAt(int xCoord, int yCoord) {
	return this->getTileAt(xCoord, yCoord)->getSurface();
}

void Map::genTerrain_Random(uint8_t maxHeight, int noiseFactor) {
	uint8_t currentHeight = 128;
	for (int i = 0; i < this->width; i++) {
		this->tileGrid.push_back(std::vector<Tile>(0));
		for (int j = 0; j < this->height; j++) {
			int r = rand() % 5;
			if (r == 0 && currentHeight > 0) {
				currentHeight--;
			} else if (r == 4 && currentHeight < 255) {
				currentHeight++;
			}
			this->tileGrid[i].push_back(Tile(i, j, currentHeight, this->erosionStrength));
		}
	}
	this->addTerrainNoise(noiseFactor);
}

void Map::genTerrain_Cliff(uint8_t maxHeight, int noiseFactor) {
	uint8_t h = 255 - noiseFactor;
	bool lowlands = false;
	for (int i = 0; i < this->width; i++) {
		this->tileGrid.push_back(std::vector<Tile>(0));
		if (i > (int)round(this->width / 2)) { h = noiseFactor; }
		for (int j = 0; j < this->height; j++) {
			this->tileGrid[i].push_back(Tile(i, j, h, this->erosionStrength));
		}
	}
	this->addTerrainNoise(noiseFactor);
}

void Map::genTerrain_Cone(uint8_t maxHeight, int noiseFactor) {
	uint8_t h = 0;
	double progress = 0.0f;
	int centerX = (int)floor(this->width / 2);
	int centerY = (int)floor(this->height / 2);
	for (int i = 0; i < this->width; i++) {
		this->tileGrid.push_back(std::vector<Tile>(0));
		for (int j = 0; j < this->height; j++) {
			double r = sqrt(pow(abs(i - centerX), 2) + pow(abs(j - centerY), 2));	// pythagorean theorem to get distance from center
			progress = r / ((double)this->width / 2.0f);
			h = 255 - (noiseFactor + (int)(progress * (maxHeight - (2 * noiseFactor))));
			this->tileGrid[i].push_back(Tile(i, j, h, this->erosionStrength));
		}
	}
	this->addTerrainNoise(noiseFactor);
}

void Map::genTerrain_Slope(uint8_t maxHeight, int noiseFactor) {
	uint8_t h = 0;
	float maxH = maxHeight;
	float progress = 0.0f;
	for (int i = 0; i < this->width; i++) {
		this->tileGrid.push_back(std::vector<Tile>(0));
		float column = i;
		float numColumns = this->width;
		progress = column / numColumns;
		h = (uint8_t)round(progress * maxH);
		for (int j = 0; j < this->height; j++) {
			this->tileGrid[i].push_back(Tile(i, j, 255-h, this->erosionStrength));
		}
	}
	this->addTerrainNoise(noiseFactor);
}

void Map::addTerrainNoise(uint8_t maxDelta) {
	int delta = 0;
	for (int i = 0; i < this->width; i++) {
		for (int j = 0; j < this->height; j++) {
			delta = (rand() % (1 + maxDelta * 2)) - maxDelta;
			if (delta < 0) {
				this->tileGrid[i][j].remSoil(abs(delta));
			} else {
				this->tileGrid[i][j].addSoil(delta);
			}
		}
	}
}
