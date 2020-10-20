#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"

#include "PugiXml\src\pugixml.hpp"

// L03: TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
struct TileSetInfo
{
    int firstGrid = 1;
    SString name = "Desert";
    int tileWidth = 32;
    int tileHeight = 32;
    int spacing = 1;
    int margin = 1;
    SDL_Texture* imgTexture;
    int imgWidth = 1;
    int imgHeight = 1;
};

// L03: TODO 1: Create a struct needed to hold the information to Map node
struct MapStruct
{
    float mapVersion = 1.0f;
    SString orientation = "orthogonal";
    SString renderorder = "right-down";
    int width = 50;
    int height = 15;
    int tileWidth = 32;
    int tileHeight = 32;
    int nextObjectId = 1;
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


    // L03: TODO 1: Add your struct for map info as public for now
    MapStruct mapInfo;

    TileSetInfo tileSetData;

    SDL_Texture* fok;

private:
    
    pugi::xml_document mapFile;
    pugi::xml_node mapVersion;
    SString folder;
    bool mapLoaded;

private:

    // Load map data
   void LoadMapData(pugi::xml_node &docInfo);

    // Load TileSet
   void LoadTileSet(pugi::xml_node& docInfo);
};

#endif // __MAP_H__