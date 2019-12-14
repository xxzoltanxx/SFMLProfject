#include "MapBuilder.h"
#include "Utilities.h"
#include <sstream>

using namespace pugi;

std::vector<MapBuilderTiled::TileMapData> MapBuilderTiled::constructTileMapData(xml_node& startingNode, xml_node& endNode)
{
	std::vector<TileMapData> tileMapData;
	auto tileSetNode = startingNode;
	while (strcmp(tileSetNode.name(), "tileset") == 0)
	{
		std::string source = tileSetNode.attribute("source").as_string();
		int firstGid = tileSetNode.attribute("firstgid").as_int();

		xml_document tilesetDocument;
		tilesetDocument.load_file((Utils::GetWorkingDirectory() + "maps/" + source).c_str());

		auto tilesetInfoNode = tilesetDocument.first_child();
		int columns = tilesetInfoNode.attribute("columns").as_int();
		int tileCount = tilesetInfoNode.attribute("tilecount").as_int();
		int tileHeight = tilesetInfoNode.attribute("tileheight").as_int();
		int tileWidth = tilesetInfoNode.attribute("tilewidth").as_int();
		std::string pngName = tilesetInfoNode.attribute("name").as_string();

		tileMapData.push_back(TileMapData(firstGid, columns, tileCount, tileHeight, tileWidth, pngName));
		tileSetNode = tileSetNode.next_sibling();
	}
	endNode = tileSetNode;
	return tileMapData;
}

TileData* MapBuilderTiled::constructTileData(int tileId, const TileMapData& data)
{
	int localId = tileId - data.firstgid;
	float startX = (localId % data.columns) * data.tileWidth;
	float startY = (localId / data.columns) * data.tileHeight;
	float sizeX = data.tileWidth;
	float sizeY = data.tileHeight;

	std::string name = data.name;
	TileData* constructedData = new TileData(sf::IntRect(startX, startY, sizeX, sizeY), sf::Vector2f(sizeX, sizeY), name);
	return constructedData;
}

void MapBuilderTiled::load(const std::string& file, Layers& mTiles, TileDataContainer& container)
{
	xml_document doc;
	doc.load_file((Utils::GetWorkingDirectory() + "maps/"+ file).c_str());
	auto tileSetNode = doc.child("map").first_child();
	xml_node& layerNode = tileSetNode;
	std::vector<TileMapData> tileMapData = constructTileMapData(tileSetNode, layerNode);


	std::unordered_set<int> addedToFileData;

	while (layerNode != nullptr && strcmp(layerNode.name(), "layer") == 0)
	{
		auto properties = layerNode.first_child();
		bool collideable = properties.first_child().attribute("value").as_bool();
		mTiles.push_back(new Layer(collideable));
		auto& currentTileMap = mTiles.back();
		for (auto iter = properties.next_sibling().begin(); iter != properties.next_sibling().end(); ++iter)
		{
			int chunkTileCounter = 0;
			auto chunk = *iter;
			int height = chunk.attribute("height").as_int();
			int width = chunk.attribute("width").as_int();
			int yStart = chunk.attribute("y").as_int();
			int xStart = chunk.attribute("x").as_int();

			std::string data = chunk.first_child().value();
			std::stringstream chunkDataStream(data);

			char delimiter;
			int tileId;

			while (chunkDataStream >> tileId)
			{
				chunkDataStream >> delimiter;
				if (tileId == 0)
				{
					chunkTileCounter++;
					continue;
				}
				if (!addedToFileData.count(tileId))
				{
					addedToFileData.emplace(tileId);
					for (auto& a : tileMapData)
					{
						if (tileId > a.firstgid)
						{
							TileData* tileData = constructTileData(tileId, a);
							container[tileId] = tileData;
						}
					}
				}
				float positionX = (xStart + chunkTileCounter % width) * container[tileId]->mSize.x - container[tileId]->mSize.x / 2.0f;
				float positionY = (yStart + chunkTileCounter / height) * container[tileId]->mSize.y - container[tileId]->mSize.y / 2.0f;
				currentTileMap->mTiles[std::make_pair(xStart + chunkTileCounter / height, yStart + chunkTileCounter % height)] = new Tile(positionX, positionY, container[tileId]);
				chunkTileCounter++;
			}
		}
		layerNode = layerNode.next_sibling();
	}
}