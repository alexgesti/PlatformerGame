//#ifndef __MAP_H__
//#define __MAP_H__
//
//#include "Module.h"
//#include "List.h"
//#include "Point.h"
//
//#include "PugiXml\src\pugixml.hpp"
//
//// L03: DONE 2: Create a struct to hold information for a TileSet
//// Ignore Terrain Types and Tile Types for now, but we want the image!
//struct TileSet
//{
//	SString	name;
//	int	firstgid;
//	int margin;
//	int	spacing;
//	int	tileWidth;
//	int	tileHeight;
//
//	SDL_Texture* texture;
//	int	texWidth;
//	int	texHeight;
//	int	numTilesWidth;
//	int	numTilesHeight;
//	int	offsetX;
//	int	offsetY;
//	int tileCount;
//
//	// L04: DONE 7: Create a method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
//	SDL_Rect GetTileRect(int id) const;
//};
//
//// L03: DONE 1: We create an enum for map type, just for convenience,
//// NOTE: Platformer game will be of type ORTHOGONAL
//enum MapTypes
//{
//	MAPTYPE_UNKNOWN = 0,
//	MAPTYPE_ORTHOGONAL,
//	MAPTYPE_ISOMETRIC,
//	MAPTYPE_STAGGERED
//};
//
//// L06: DONE 5: Create a generic structure to hold properties
//struct Properties
//{
//	struct Property
//	{
//		SString	name;
//		int value;
//	};
//	
//	~Properties()
//	{
//	}
//
//	// L06: DONE 7: Method to ask for the value of a custom property
//	int GetProperty(const char* value, int defaultValue = 0) const;
//
//	List<Property*> list;
//};
//
//// L04: DONE 1: Create a struct for the map layer
//struct MapLayer
//{
//	SString	name;
//	int width;
//	int height;
//	uint* data;
//
//	// L06: DONE 1: Support custom properties
//	Properties properties;
//
//	MapLayer() : data(NULL)
//	{}
//
//	~MapLayer()
//	{
//		RELEASE(data);
//	}
//
//	// L04: DONE 6: Short function to get the value of x,y
//	inline uint Get(int x, int y) const
//	{
//		return data[(y * width) + x];
//	}
//};
//
//// L03: DONE 1: Create a struct needed to hold the information to Map node
//struct MapData
//{
//	int width;
//	int	height;
//	int	tileWidth;
//	int	tileHeight;
//	SString orientation;
//	SDL_Color backgroundColor;
//	MapTypes type;
//	List<TileSet*> tilesets;
//
//	// L04: DONE 2: Add a list/array of layers to the map
//	List<MapLayer*> layers;
//};
//
//class Map : public Module
//{
//public:
//
//    Map();
//
//    // Destructor
//    virtual ~Map();
//
//    // Called before render is available
//    bool Awake(pugi::xml_node& conf);
//
//    // Called each loop iteration
//    void Draw();
//
//    // Called before quitting
//    bool CleanUp();
//
//    // Load new map
//    bool Load(const char* path);
//
//	// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
//	iPoint MapToWorld(int x, int y) const;
//
//	// L05: DONE 2: Add orthographic world to map coordinates
//	iPoint WorldToMap(int x, int y) const;
//
//private:
//
//	// L03: Methods to load all required map data
//	bool LoadMap();
//	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
//	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
//	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
//
//	// L06: DONE 6: Load a group of properties 
//	bool LoadProperties(pugi::xml_node& node, Properties& properties);
//
//	// L06: DONE 3: Pick the right Tileset based on a tile id
//	TileSet* GetTilesetFromTileId(int id) const;
//
//public:
//
//    // L03: DONE 1: Add your struct for map info
//	MapData data;
//
//private:
//
//    pugi::xml_document mapFile;
//    SString folder;
//    bool mapLoaded;
//};
//
//#endif // __MAP_H__

#ifndef __MAP_H__
#define __MAP_H__

#include "External/PugiXml/src/pugixml.hpp"
#include "List.h"
#include "Point.h"
#include "SString.h"
#include "Module.h"

union value {
	const char* vString;
	int				vInt;
	float			vFloat;
};

struct Properties //Properties
{
	struct Property
	{
		SString	name;
		value		data;

	};

	Properties::~Properties()															//Deletes every property and frees all allocated memory.
	{
		ListItem<Property*>* item;
		item = propertyList.start;
		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}
		propertyList.clear();												//Clears propertyList by deleting all items in the list and freeing all allocated memory.
	}

	value Get(const char* name, value* defaultValue = nullptr) const;

	List<Property*>	propertyList;
};

// ----------------------------------------------------
struct MapLayer
{
	SString	name;
	int			width;
	int			height;
	uint* data;
	Properties	properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	//Get id of tile in position x, y from data[] array
	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
	}
};

// ----------------------------------------------------
struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	SString			name;
	int					firstgid;
	int					tileWidth;
	int					tileHeight;
	int					margin;
	int					spacing;
	SDL_Texture* texture;
	int					texWidth;
	int					texHeight;
	int					numTilesWidth;
	int					numTilesHeight;
	int					offsetX;
	int					offsetY;

	SDL_Rect* playerTileRect = new SDL_Rect;
	SDL_Rect* PlayerTileRect(uint tileId) {

		SDL_Rect* ret = playerTileRect;

		int numTWidth = texWidth / tileWidth;
		int numTHeight = texHeight / tileHeight;

		int x = tileId % numTWidth;
		int y = tileId / numTWidth;


		ret->x = x * tileWidth;
		ret->y = y * tileHeight;
		ret->w = tileWidth;
		ret->h = tileHeight;

		return ret;
	}

};

enum class MapTypes
{
	UNKNOWN = 0,
	ORTHOGONAL,
	ISOMETRIC,
	STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int						width;
	int						height;
	int						tileWidth;
	int						tileHeight;

	SString				name;
	Point<float>			startingPosition;

	SDL_Color				backgroundColor;
	MapTypes				type;
	List<TileSet*>		tilesets;
	List<MapLayer*>		layers;
};

// ----------------------------------------------------
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

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Load new map
	bool Load(const char* path);

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tilesetNode, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tilesetNode, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	TileSet* GetTilesetFromTileId(int id) const;

public:

	MapData data;

private:

	pugi::xml_document	mapFile;
	SString			folder;
	bool				mapLoaded;
};

#endif // __MAP_H__