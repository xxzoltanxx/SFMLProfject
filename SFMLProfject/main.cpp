#include <unordered_map>
#include <vector>
#include <functional>

#include "Game.h"

int main()
{
	Game game;
	game.run();
}

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
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

using Tiles = std::array<std::array<Tile, 512>, 512>;
using TileDataContainer = std::unordered_map<unsigned int, TileData>;

class MapBuilder
{
	virtual void load(const std::string& file, Tiles& mTiles, TileDataContainer& container) = 0;
};

class MapBuilderTiled : public MapBuilder
{
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

using namespace rapidxml;
void MapBuilderTiled::load(const std::string& file, Tiles& mTiles, TileDataContainer& container)
{
	rapidxml::file<> xmlFile((Utils::GetWorkingDirectory() + file).c_str());
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());

	xml_node<>* mapNode = doc.first_node;
	xml_node<>* tileSetNode = mapNode->first_node;
	std::vector<TileMapData> tileMapData;
	while (strcmp(tileSetNode->name, "tileset") == 0)
	{
		std::string fileTileset = tileSetNode->first_attribute.value;
		xml_attribute<>* fileNameTileset = tileSetNode->first_attribute;
		int gid = std::stoi(fileNameTileset->next_attribute.value);
		rapidxml::file<> xmlFileTileset((Utils::GetWorkingDirectory() + fileTileset).c_str());
		rapidxml::xml_document<> docTileset;
		docTileset.parse<0>(xmlFileTileset.data());

		xml_node<>* tilesetNodeFile = docTileset.first_node;
		xml_attribute<>* columns = tilesetNodeFile->first_attribute;
		xml_attribute<>* tileCount = columns->next_attribute;
		xml_attribute<>* tileHeight = tileCount->next_attribute;
		xml_attribute<>* tileWidth = tileHeight->next_attribute;
		xml_attribute<>* name = tileWidth->next_attribute;
		tileMapData.push_back(TileMapData(gid, std::stoi(columns->value), std::stoi(tileCount->value), std::stoi(tileHeight->value), std::stoi(tileWidth->value), name->value));
		tileSetNode = tileSetNode->next_sibling;
	}
	xml_node<>* layer = tileSetNode->next_sibling;
	do
	{
		xml_attribute<>* height = layer->first_attribute;
		xml_attribute<>* width = height->next_attribute;
		xml_attribute<>
	}
}

class Map
{
public:
	void loadMap(const std::string& map);

private:
	MapBuilder* builder = new MapBuilderTiled();
	Tiles mTiles;
	TileDataContainer mTileData;
};