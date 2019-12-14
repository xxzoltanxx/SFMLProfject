#pragma once
#include "Map.h"
#include "pugixml.hpp"

class MapBuilder
{
public:
	virtual void load(const std::string& file, Layers& mTiles, TileDataContainer& container) = 0;
};


class MapBuilderTiled : public MapBuilder
{
public:
	void load(const std::string& file, Layers& mTiles, TileDataContainer& container) override;
private:
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
			rows = tileCount / columns;
		}
		int firstgid;
		int columns;
		int tileCount;
		int tileHeight;
		int tileWidth;
		int rows;
		std::string name;
	};
	std::vector<TileMapData> constructTileMapData(pugi::xml_node& startingNode, pugi::xml_node& endNode);
	TileData* constructTileData(int tileId, const TileMapData& data);
};