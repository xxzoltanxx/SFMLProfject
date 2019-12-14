#pragma once
#include <unordered_set>
#include "TextureManager.h"
#include "Utilities.h"

struct TileData
{
	TileData(const sf::IntRect& textureRect, const sf::Vector2f& size, const std::string& tileSet);
	~TileData();
	std::string tileSheetName;
	sf::Sprite tileSprite;
	sf::Vector2f mSize;
};

struct Tile
{
	Tile(float x, float y, TileData* data);
	float mX;
	float mY;
	TileData* mData;
};

struct Layer
{
	Layer(const bool& col) : mIsCollidable(col) {}
	std::unordered_map<std::pair<int, int>, Tile*, pair_hash> mTiles;
	bool mIsCollidable;

	~Layer()
	{
		for (auto& tile : mTiles)
		{
			delete tile.second;
		}
	}
};

using TileDataContainer = std::unordered_map<unsigned int, TileData*>;
using Layers = std::vector<Layer*>;

class MapBuilder;

class Map
{
public:
	Map();
	void clearMap();
	void loadMap(const std::string& file);

	void draw(sf::RenderTarget& target);

private:
	bool isValidTile(const Tile& tile) const;

	MapBuilder* builder;
	TileDataContainer mTileDataContainer;
	Layers mLayers;
};