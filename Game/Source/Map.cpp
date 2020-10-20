
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"

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

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    folder.Create(config.child("folder").child_value());

    Load("hello2.tmx");

    return ret;
}

void Map::Draw()
{
    if(mapLoaded == false)
        return;

    // L03: TODO 6: Iterate all tilesets and draw all their 
    // images in 0,0 (you should have only one tileset for now)

    tileSetData.imgTexture = app->tex->Load("Assets/maps/tmw_desert_spacing.png");
    for (pugi::xml_node tileset = mapVersion.child("tileset"); tileset; tileset = tileset.next_sibling("tileset"))
    {
        app->render->DrawTexture(tileSetData.imgTexture,0,0);
    }
   
    
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    // L03: TODO 2: Make sure you clean up any memory allocated from tilesets/map

    
    mapFile.reset();
    
    return true;
}

// Load new map
bool Map::Load(const char* filename)
{
    bool ret = true;
    SString tmp("%s%s", folder.GetString(), filename);
   
    pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
        ret = false;
    }
    else
    {
        mapVersion = mapFile.child("map");
    }

    if(ret == true)
    {
        // L03: TODO 3: Create and call a private function to load and fill all your map data
        LoadMapData(mapVersion);
    }

    // L03: TODO 4: Create and call a private function to load a tileset
    // remember to support more any number of tilesets!
    if (ret == true)
    {
        LoadTileSet(mapVersion);
    }

    if(ret == true)
    {
        // L03: TODO 5: LOG all the data loaded iterate all tilesets and LOG everything
        LOG("Succesfully parsed map XML file: hello2.tmx");
        LOG("width: %d height: %d", mapInfo.width, mapInfo.height);
        LOG("tile_width: %d tile_height: %d", tileSetData.tileWidth, tileSetData.tileHeight);
        LOG("Tileset ----");
        LOG("name: %s firstgid: %s", tileSetData.name, tileSetData.firstGrid);
        LOG("tile width: %d tile height: %d", tileSetData.tileWidth, tileSetData.tileHeight);
        LOG("spacing: %d margin: %d", tileSetData.spacing, tileSetData.margin);
    }

    mapLoaded = ret;

    return ret;
}

void Map::LoadTileSet(pugi::xml_node& map)
{
    
    for (pugi::xml_node tileset = map.child("tileset"); tileset; tileset = tileset.next_sibling("tileset"))
    {
        tileSetData.firstGrid = map.child("tileset").attribute("firstgid").as_int(1);
        tileSetData.margin = map.child("tileset").attribute("margin").as_int(1);
        tileSetData.name = map.child("tileset").attribute("name").as_string();
        tileSetData.spacing = map.child("tileset").attribute("spacing").as_int(1);
        tileSetData.tileHeight = map.child("tileset").attribute("tileheight").as_int(32);
        tileSetData.tileWidth = map.child("tileset").attribute("tilewidth").as_int(32);
        tileSetData.imgTexture = app->tex->Load(map.child("image").attribute("source").as_string("/Assets/maps/tmw_desert_spacing.png"));
        tileSetData.imgWidth = map.child("image").attribute("width").as_int(32);
        tileSetData.imgHeight = map.child("image").attribute("height").as_int(32);
    }
}

void Map::LoadMapData(pugi::xml_node& map)
{
     
    mapInfo.height = map.child("map").attribute("height").as_int(1);
    mapInfo.mapVersion = map.child("map").attribute("version").as_float(1.4f);
    mapInfo.nextObjectId = map.child("map").attribute("nextobjectid").as_int(1);
    mapInfo.orientation = map.child("map").attribute("orientation").as_string("orthogonal");
    mapInfo.renderorder = map.child("map").attribute("renderorder").as_string("right_down");
    mapInfo.tileHeight = map.child("map").attribute("tileheight").as_int(32);
    mapInfo.tileWidth = map.child("map").attribute("tilewidth").as_int(32);
    mapInfo.width = map.child("map").attribute("width").as_int(15);
  
}   