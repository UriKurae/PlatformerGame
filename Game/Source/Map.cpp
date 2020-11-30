
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Player.h"
#include "Input.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>

Map::Map() : Module(), mapLoaded(false)
{
	name.Create("map");
}

// Destructor
Map::~Map()
{}


int Properties::GetProperty(const char* value, int defaultValue) const
{

	ListItem<Properties::Property*>* item = list.start;

	while (item != NULL)
	{
		if (item->data->name == value)
		{
			return item->data->value;
		}
		item = item->next;
	}

	return defaultValue;
}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;
	
	folder.Create(config.child("folder").child_value());

	return ret;
}

void Map::Draw()
{
	if (mapLoaded == false) return;

	ListItem<MapLayer*>* layer = data.layers.start;

	TileSet* set;

	while (layer != NULL)
	{
		int noDraw = layer->data->properties.GetProperty("Nodraw");

		if (noDraw == 0 || viewHitboxes == true)
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = layer->data->Get(x, y);
					if (tileId > 0)
					{
						set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint coords = MapToWorld(x, y);
						app->render->DrawTexture(set->texture, coords.x, coords.y, &rect);
					}
				}
			}
		}
		layer = layer->next;
	}

}
iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	if (data.type == MapTypes::MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tileWidth;
		ret.y = y * data.tileHeight;
	}
	else
	{
		ret.x = (x - y) * (data.tileWidth * 0.5f);
		ret.y = (x + y) * (data.tileWidth * 0.5f);
	}

	return ret;
}


iPoint Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	if (data.type == MapTypes::MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tileWidth;
		ret.y = y / data.tileHeight;
	}
	else
	{
		ret.x = (x + y) / (data.tileWidth / 0.5f);
		ret.y = (x - y) / (data.tileWidth / 0.5f);
	}
	

	return ret;
}

TileSet* Map::GetTilesetFromTileId(int id) const
{
	ListItem<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	while (item != NULL)
	{
		if (id < ((set->numTilesHeight * set->numTilesWidth) + set->firstgid))
		{
			break;
		}
		item = item->next;
		if (item != NULL) set = item->data;
	}

	return set;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };

	int relativeId = id - firstgid;
	rect.w = tileWidth;
	rect.h = tileHeight;
	rect.x = margin + ((rect.w + spacing) * (relativeId % numTilesWidth));
	rect.y = margin + ((rect.h + spacing) * (relativeId / numTilesWidth));

	return rect;
}

// Called before quitting
bool Map::CleanUp()
{
	LOG("Unloading map");

	ListItem<TileSet*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.Clear();

	// Remove all layers
	ListItem<MapLayer*>* item2;
	item2 = data.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.Clear();

	// Clean up the pugui tree
	mapFile.reset();

	return true;
}

// Load new map
bool Map::Load(const char* filename)
{
	bool ret = true;
	SString tmp("%s%s", folder.GetString(), filename);

	pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
		ret = false;
	}

	// Load general info
	if (ret == true)
	{
		
		ret = LoadMap();
	}

	pugi::xml_node tileset;
	for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true) ret = LoadTilesetDetails(tileset, set);

		if (ret == true) ret = LoadTilesetImage(tileset, set);

		data.tilesets.Add(set);
	}

	// Load layer info
	pugi::xml_node layer;
	for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if (ret == true)
			data.layers.Add(lay);
	}

	if (ret == true)
	{
		LOG("Map Info -----------------------");

		LOG("Succesfully parsed map XML file: %s", filename);
		LOG("Map width: %d; Map height: %d", mapFile.child("map").attribute("width").as_int(), mapFile.child("map").attribute("height").as_int());
		LOG("Orientation: %s", mapFile.child("map").attribute("orientation").as_string());

		LOG("Tilesets Info -----------------------");

		for (tileset = mapFile.child("map").child("tileset"); tileset; tileset = tileset.next_sibling("tileset"))
		{
			LOG("Succesfully loaded tileset with name: %s", tileset.attribute("name").as_string());
			LOG("Tile width: %d; Tile height: %d", tileset.attribute("tilewidth").as_int(), tileset.attribute("tileheight").as_int());
			LOG("Firstgid: %d", tileset.attribute("firstgid").as_int());
			LOG("Tile count: %d; Columns: %d", tileset.attribute("tilecount").as_int(), tileset.attribute("columns").as_int());
		}
	   
		LOG("Layers Info -----------------------");

		for (layer = mapFile.child("map").child("layer"); layer; layer = layer.next_sibling("layer"))
		{
			LOG("Loaded Layer: %s ", layer.attribute("name").as_string());
			LOG("Layer width: %d; Layer height: %d", layer.attribute("width").as_int(), layer.attribute("height").as_int());
			LOG("Layer id: %d", layer.attribute("id").as_int());
		}
	}

	mapLoaded = ret;

	return ret;
}

bool Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = mapFile.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.height = map.attribute("height").as_int();
		data.width = map.attribute("width").as_int();
		data.tileHeight = map.attribute("tileheight").as_int();
		data.tileWidth = map.attribute("tilewidth").as_int();

		SString tmp("%s", map.attribute("orientation").value());

		
		if (tmp == "orthogonal")
		{
			data.type = MapTypes::MAPTYPE_ORTHOGONAL;
		}
		else if (tmp == "isometric")
		{
			data.type = MapTypes::MAPTYPE_ISOMETRIC;
		}
		else if (tmp == "staggered")
		{
			data.type = MapTypes::MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MapTypes::MAPTYPE_UNKNOWN;
		}

	}

	return ret;
}

bool Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	 
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->name = tileset_node.attribute("name").value();
	set->tileWidth = tileset_node.attribute("tilewidth").as_int();
	set->tileHeight = tileset_node.attribute("tileheight").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	set->margin = tileset_node.attribute("margin").as_int();

	return ret;
}

bool Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		SString imgSource;
		imgSource.Create("%s%s", folder.GetString(), image.attribute("source").value());

		set->texture = app->tex->Load(imgSource.GetString());
		set->texWidth = image.attribute("width").as_int();
		set->texHeight = image.attribute("height").as_int();
		set->numTilesWidth = set->texWidth / set->tileWidth;
		set->numTilesHeight = set->texHeight / set->tileHeight;
	}

	return ret;
}


bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->height = node.attribute("height").as_int();
	layer->width = node.attribute("width").as_int();
	layer->name = node.attribute("name").value();

	int newSize = layer->width * layer->height;

	layer->data = new uint[newSize];

	pugi::xml_node sibling = node.child("data").child("tile");

	for (int i = 0; i < newSize; i++)
	{
		layer->data[i] = sibling.attribute("gid").as_int();
		sibling = sibling.next_sibling("tile");
	}

	LoadProperties(node, layer->properties);

	return ret;
}

bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	for (pugi::xml_node n = node.child("properties").child("property"); n; n = n.next_sibling("property"))
	{
		Properties::Property* item = new Properties::Property();
		item->name = n.attribute("name").value();
		item->value = n.attribute("value").as_int();

		properties.list.Add(item);
	}
	
	return ret;
}