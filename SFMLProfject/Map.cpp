#include "Map.h"
#include "MapBuilder.h"

TileData::TileData(const sf::IntRect& textureRect, const sf::Vector2f& size, const std::string& tileSet)
{
	tileSprite.setTexture(*TextureManager::get()->request(tileSet));
	tileSprite.setTextureRect(textureRect);
	mSize = size;
	tileSheetName = tileSet;
}

TileData::~TileData()
{
	TextureManager::get()->release(tileSheetName);
}

Tile::Tile(float x, float y, TileData* data)
{
	mX = x;
	mY = y;
	mData = data;
}

Map::Map()
{
	builder = new MapBuilderTiled();
}

void Map::clearMap()
{
	for (auto& data : mTileDataContainer)
	{
		delete data.second;
	}

	for (auto& layer : mLayers)
	{
		delete layer;
	}

	mLayers.clear();
	mTileDataContainer.clear();
}

void Map::loadMap(const std::string& file)
{
	builder->load(file, mLayers, mTileDataContainer);
}

void Map::draw(sf::RenderTarget& target)
{
	//TODO DRAW ONLY IF ITS INSIDE THE RENDER WINDOW
	for (auto layer = mLayers.begin(); layer != mLayers.end(); ++layer)
	{
		for (auto& tile : (*layer)->mTiles)
		{
			sf::Sprite& spriteFlyweight = tile.second->mData->tileSprite;
			spriteFlyweight.setPosition(tile.second->mX, tile.second->mY);
			target.draw(spriteFlyweight);
		}
	}
}