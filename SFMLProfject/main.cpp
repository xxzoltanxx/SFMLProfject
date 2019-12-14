#include <unordered_map>
#include <vector>
#include <functional>

#include "Game.h"

#include "Utilities.h"
#include <array>

struct TileData
{
	sf::Sprite tileSprite;
	sf::Vector2f mSize;
};

struct Tile
{
	float mX;
	float mY;
	TileData* mData;
};

using Tiles = std::unordered_map<unsigned int, Tile*>;
using TileDataContainer = std::unordered_map<unsigned int, TileData>;

class MapBuilder
{
	virtual void load(const std::string& file, Tiles& mTiles, TileDataContainer& container) = 0;
};

class MapBuilderTiled : public MapBuilder
{
public:
	struct TileMapData
	{
		TileMapData(const int& gid, const int& col, const int& tC, const int& tH, const int& tW, const std::string& nme)
		{
			columns = col;
			tileCount = tC;
			tileHeight = tH;
			tileWidth = tW;
			name = nme;
			firstgid = gid;
		}
		int firstgid;
		int columns;
		int tileCount;
		int tileHeight;
		int tileWidth;
		std::string name;
	};
	void load(const std::string& file, Tiles& mTiles, TileDataContainer& container) override;
};

void MapBuilderTiled::load(const std::string& file, Tiles& mTiles, TileDataContainer& container)
{
}

class Map
{
public:
	void loadMap(const std::string& map) {};

private:
	MapBuilder* builder = new MapBuilderTiled();
	Tiles mTiles;
	TileDataContainer mTileData;
};

int main()
{
	Tiles tiles;
	TileDataContainer tiledata;
	MapBuilderTiled tiled;
	tiled.load("testmap.xml", tiles, tiledata);
	Game game;
	game.run();
}