#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "DynArray.h"
#include "PQueue.h"

#include "PugiXml\src\pugixml.hpp"
#include "SDL/include/SDL.h"

#define COST_MAP_SIZE	100

struct TileSet
{
	SString name;
	int firstgid;
	int margin;
	int spacing;
	int tileWidth;
	int tileHeight;

	SDL_Texture* texture;
	int texWidth;
	int texHeight;
	int numTilesWidth;
	int numTilesHeight;
	int offsetX;
	int offsetY;

	SDL_Rect GetTileRect(int id) const;

};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

struct Properties
{
	struct Property
	{
		//...
		SString name;
		int value;
	};

	~Properties()
	{
		
	}

	
	int GetProperty(const char* name, int default_value = 0) const;

	List<Property*> list;
};

struct MapLayer
{
	SString name;
	int width;
	int height;
	uint* data;
	

	Properties properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
	}
};


struct MapData
{
	int width;
	int height;
	int tileWidth;
	int tileHeight;
	SDL_Color backgroundColor;
	MapTypes type;
	List<TileSet*> tilesets;

	List<MapLayer*> layers;
};

class Map : public Module
{
public:

	Map();

	// Destructor
	virtual ~Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	iPoint MapToWorld(int x, int y) const;

	iPoint WorldToMap(int x, int y) const;


	TileSet* GetTilesetFromTileId(int id) const;

private:

	// Methods for loading the maps
	
	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadProperties(pugi::xml_node& node, Properties& properties);
	bool ScreenLimits();


public:

	MapData data;
	bool viewHitboxes = false;

private:

	pugi::xml_document mapFile;
	SString folder;
	bool mapLoaded;

	PQueue<iPoint> frontier;
	List<iPoint> visited;
	uint newCost;
	iPoint pathGoal;

	List<iPoint> breadcrumbs;
	uint costSoFar[COST_MAP_SIZE][COST_MAP_SIZE];
	DynArray<iPoint> path;

	// L12: AStar (A*) variables
	iPoint goalAStar;			// Store goal target tile
	bool finishAStar = false;	// Detect when reached goal

};

#endif // __MAP_H__