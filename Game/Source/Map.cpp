//Ver.2
#include "Defs.h"
#include "Log.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
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

	return ret;
}

bool Map::Save(pugi::xml_node& node) const
{
	LOG("Saving Map...");
	node.append_child("map_name").append_attribute("filename") = data.name.GetString();

	return true;
}

bool Map::Load(pugi::xml_node& node)
{
	LOG("Loading Map...");

	Load(node.child("map_name").attribute("filename").as_string());


	return true;
}

void Map::Draw()
{
	if (mapLoaded == false)
		return;

	// TODO 4: Make sure we draw all the layers and not just the first one
	ListItem<MapLayer*>* layer = data.layers.start;
	while (layer != nullptr) {
		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int tileId = layer->data->Get(x, y);
				if (tileId > 0)
				{
					TileSet* tileset = GetTilesetFromTileId(tileId);
					if (tileset != nullptr)
					{
						SDL_Rect r = tileset->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);

						app->render->DrawTexture(tileset->texture, pos.x, pos.y, &r);
					}
				}
			}
		}
		layer = layer->next;
	}
}

TileSet* Map::GetTilesetFromTileId(int id) const
{
	// TODO 3: Complete this method so we pick the right
	// Tileset based on a tile id //data.tilesets.start->data
	TileSet* ret = nullptr;

	ListItem<TileSet*>* i = data.tilesets.start;
	while (i->next != nullptr) 
	{
		if (id >= i->data->firstgid && id < i->next->data->firstgid) 
		{
			ret = i->data;
			break;
		}
		i = i->next;
	}
	if (ret == nullptr) 
	{
		ret = data.tilesets.end->data;
	}

	return ret;
}

iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	if (data.type == MapTypes::ORTHOGONAL)
	{
		ret.x = x * data.tileWidth;
		ret.y = y * data.tileHeight;
	}
	else if (data.type == MapTypes::ISOMETRIC)
	{
		ret.x = (x - y) * (data.tileWidth * 0.5f);
		ret.y = (x + y) * (data.tileHeight * 0.5f);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

iPoint Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	if (data.type == MapTypes::ORTHOGONAL)
	{
		ret.x = x / data.tileWidth;
		ret.y = y / data.tileHeight;
	}

	else if (data.type == MapTypes::ISOMETRIC)
	{
		float halfWidth = data.tileWidth * 0.5f;
		float halfHeight = data.tileHeight * 0.5f;
		ret.x = int((x / halfWidth + y / halfHeight) / 2);
		ret.y = int((y / halfHeight - (x / halfWidth)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relativeId = id - firstgid;
	SDL_Rect rect;
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

	// Remove all tilesets
	ListItem<TileSet*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		delete item->data->playerTileRect;
		

		SDL_DestroyTexture(item->data->texture);

		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// Remove all layers
	ListItem<MapLayer*>* item2;
	item2 = data.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.clear();

	// Clean up the pugui tree
	mapFile.reset();

	return true;
}

// Load new map
bool Map::Load(const char* fileCName)
{
	bool ret = true;
	SString tmp("%s%s", folder.GetString(), fileCName);

	SString* fileName = new SString(fileCName);

	pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", fileCName, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if (ret == true)
	{
		data.name = fileName->GetString();
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if (ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* mapLayers = new MapLayer();

		ret = LoadLayer(layer, mapLayers);

		if (ret == true)
			data.layers.add(mapLayers);
	}

	if (ret == true)
	{
		LOG("Successfully parsed map XML file: %s", fileCName);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tileWidth: %d tileHeight: %d", data.tileWidth, data.tileHeight);

		ListItem<TileSet*>* item = data.tilesets.start;
		while (item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tileWidth, s->tileHeight);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		ListItem<MapLayer*>* itemLayer = data.layers.start;
		while (itemLayer != NULL)
		{
			MapLayer* l = itemLayer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			itemLayer = itemLayer->next;
		}
	}

	mapLoaded = ret;

	return ret;
}

// Load map general properties
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
		data.startingPosition.x = map.child("properties").child("property").attribute("value").as_float();
		data.startingPosition.y = map.child("properties").child("property").next_sibling().attribute("value").as_float();

		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tileWidth = map.attribute("tilewidth").as_int();
		data.tileHeight = map.attribute("tileheight").as_int();
		SString bgColor(map.attribute("backgroundcolor").as_string());

		data.backgroundColor.r = 0;
		data.backgroundColor.g = 0;
		data.backgroundColor.b = 0;
		data.backgroundColor.a = 0;

		if (bgColor.Length() > 0)
		{
			SString red, green, blue;
			bgColor.SubString(1, 2, red);
			bgColor.SubString(3, 4, green);
			bgColor.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.backgroundColor.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.backgroundColor.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.backgroundColor.b = v;
		}

		SString orientation(map.attribute("orientation").as_string());

		if (orientation == "orthogonal")
		{
			data.type = MapTypes::ORTHOGONAL;
		}
		else if (orientation == "isometric")
		{
			data.type = MapTypes::ISOMETRIC;
		}
		else if (orientation == "staggered")
		{
			data.type = MapTypes::STAGGERED;
		}
		else
		{
			data.type = MapTypes::UNKNOWN;
		}
	}

	return ret;
}

// Load Tileset data
bool Map::LoadTilesetDetails(pugi::xml_node& tilesetNode, TileSet* set)
{
	bool ret = true;
	set->name.Create(tilesetNode.attribute("name").as_string());
	set->firstgid = tilesetNode.attribute("firstgid").as_int();
	set->tileWidth = tilesetNode.attribute("tilewidth").as_int();
	set->tileHeight = tilesetNode.attribute("tileheight").as_int();
	set->margin = tilesetNode.attribute("margin").as_int();
	set->spacing = tilesetNode.attribute("spacing").as_int();
	pugi::xml_node offset = tilesetNode.child("tileoffset");

	if (offset != NULL)
	{
		set->offsetX = offset.attribute("x").as_int();
		set->offsetY = offset.attribute("y").as_int();
	}
	else
	{
		set->offsetX = 0;
		set->offsetY = 0;
	}

	return ret;
}

//Load Tileset Image
bool Map::LoadTilesetImage(pugi::xml_node& tilesetNode, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tilesetNode.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = app->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->texWidth = image.attribute("width").as_int();

		if (set->texWidth <= 0)
		{
			set->texWidth = w;
		}

		set->texHeight = image.attribute("height").as_int();

		if (set->texHeight <= 0)
		{
			set->texHeight = h;
		}

		set->numTilesWidth = set->texWidth / set->tileWidth;
		set->numTilesHeight = set->texHeight / set->tileHeight;
	}

	return ret;
}

//Load different map layers
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	LoadProperties(node, layer->properties);
	pugi::xml_node layerData = node.child("data");

	if (layerData == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->data = new uint[layer->width*layer->height];
		memset(layer->data, 0, layer->width*layer->height);

		int i = 0;
		for (pugi::xml_node tile = layerData.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			layer->data[i++] = tile.attribute("gid").as_int(0);
		}
	}

	return ret;
}

// Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)							//REVISE THIS HERE. Check why it crashes the game at exit time.
{
	bool ret = false;

	pugi::xml_node data = node.child("properties");

	if (data != nullptr)
	{
		pugi::xml_node property;

		for (property = data.child("property"); property; property = property.next_sibling("property"))
		{
			Properties::Property* p = new Properties::Property();

			p->name = property.attribute("name").as_string();

			properties.propertyList.add(p);
		}
	}

	return ret;
}

// Returns a property
value Properties::Get(const char* name, value* defaultValue) const
{
	ListItem<Property*>* item = propertyList.start;

	while (item)
	{
		if (item->data->name == name)
			return item->data->data;
		item = item->next;
	}

	return *defaultValue;
}

//Ver. 1
//#include "App.h"
//#include "Render.h"
//#include "Textures.h"
//#include "Map.h"
//
//#include "Defs.h"
//#include "Log.h"
//
//#include <math.h>
//
//Map::Map() : Module(), mapLoaded(false)
//{
//    name.Create("map");
//}
//
//// Destructor
//Map::~Map()
//{}
//
//// L06: TODO 7: Ask for the value of a custom property
//int Properties::GetProperty(const char* value, int defaultValue) const
//{
//	for (int i = 0; i < list.count(); i++)
//	{
//		if (strcmp(list.At(i)->data->name.GetString(), value) == 0)
//		{
//			if (list.At(i)->data->value != defaultValue) 
//				return list.At(i)->data->value;
//		}
//	}
//
//	return defaultValue;
//}
//
//// Called before render is available
//bool Map::Awake(pugi::xml_node& config)
//{
//    LOG("Loading Map Parser");
//    bool ret = true;
//
//    folder.Create(config.child("folder").child_value());
//
//    return ret;
//}
//
//// Draw the map (all requried layers)
//void Map::Draw()
//{
//	if (mapLoaded == false)
//		return;
//
//	// TODO 4: Make sure we draw all the layers and not just the first one
//	ListItem<MapLayer*>* layer = data.layers.start;
//	while (layer != nullptr) {
//		for (int y = 0; y < data.height; ++y)
//		{
//			for (int x = 0; x < data.width; ++x)
//			{
//				int tileId = layer->data->Get(x, y);
//				if (tileId > 0)
//				{
//					TileSet* tileset = GetTilesetFromTileId(tileId);
//					if (tileset != nullptr)
//					{
//						SDL_Rect r = tileset->GetTileRect(tileId);
//						iPoint pos = MapToWorld(x, y);
//
//						app->render->DrawTexture(tileset->texture, pos.x, pos.y, &r, SDL_FLIP_NONE);
//					}
//				}
//			}
//		}
//		layer = layer->next;
//	}
//}
//
//// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
//iPoint Map::MapToWorld(int x, int y) const
//{
//	iPoint ret(0, 0);
//
//	// L05: DONE 1: Add isometric map to world coordinates
//	if (data.type == MAPTYPE_ORTHOGONAL)
//	{
//		ret.x = x * data.tileWidth;
//		ret.y = y * data.tileHeight;
//	}
//	else if (data.type == MAPTYPE_ISOMETRIC)
//	{
//		ret.x = (x - y) * (data.tileWidth / 2);
//		ret.y = (x + y) * (data.tileHeight / 2);
//	}
//	else
//	{
//		LOG("Unknown map type(M2W)\n");
//		ret.x = x; ret.y = y;
//	}
//
//	return ret;
//}
//
//// L05: DONE 2: Add orthographic world to map coordinates
//iPoint Map::WorldToMap(int x, int y) const
//{
//	iPoint ret;
//
//	// L05: DONE 3: Add the case for isometric maps to WorldToMap
//	if (data.type == MAPTYPE_ORTHOGONAL)
//	{
//		ret.x = x / data.tileWidth;
//		ret.y = y / data.tileHeight;
//	}
//	else if (data.type == MAPTYPE_ISOMETRIC)
//	{
//
//		float half_width = data.tileWidth * 0.5f;
//		float half_height = data.tileHeight * 0.5f;
//		ret.x = int((x / half_width + y / half_height) / 2);
//		ret.y = int((y / half_height - (x / half_width)) / 2);
//	}
//	else
//	{
//		LOG("Unknown map type (W2M)\n");
//		ret.x = x; ret.y = y;
//	}
//
//	return ret;
//}
//
//// L06: DONE 3: Pick the right Tileset based on a tile id
//TileSet* Map::GetTilesetFromTileId(int id) const
//{
//	/*ListItem<TileSet*>* item = data.tilesets.start;
//	TileSet* set = item->data;
//
//	for (set; set; item = item->next, set = item->data)
//	{
//		if (id >= set->firstgid && id < set->firstgid + set->tileCount)
//			return set;
//	}
//
//	return set;*/
//
//	TileSet* ret = nullptr;
//	ListItem<TileSet*>* item = data.tilesets.start;
//
//	while (item->next != nullptr)
//	{
//		if (id >= item->data->firstgid && id < item->next->data->firstgid)
//		{
//			ret = item->data;
//			break;
//		}
//		item = item->next;
//	}
//
//	if (ret == nullptr)
//	{
//		ret = data.tilesets.end->data;
//	}
//
//	return ret;
//}
//
//// Get relative Tile rectangle
//SDL_Rect TileSet::GetTileRect(int id) const
//{
//	SDL_Rect rect = { 0 };
//
//	// L04: DONE 7: Get relative Tile rectangle
//	int relativeId = id - firstgid;
//	rect.w = tileWidth;
//	rect.h = tileHeight;
//	rect.x = margin + ((rect.w + spacing) * (relativeId % numTilesWidth));
//	rect.y = margin + ((rect.h + spacing) * (relativeId / numTilesWidth));
//	
//	return rect;
//}
//
//// Called before quitting
//bool Map::CleanUp()
//{
//    LOG("Unloading map");
//
//    // L03: DONE 2: Make sure you clean up any memory allocated from tilesets/map
//    // Remove all tilesets
//	ListItem<TileSet*>* item;
//	item = data.tilesets.start;
//
//	while (item != NULL)
//	{
//		RELEASE(item->data);
//		item = item->next;
//	}
//	data.tilesets.clear();
//
//	// L04: DONE 2: clean up all layer data
//	// Remove all layers
//	ListItem<MapLayer*>* item2;
//	item2 = data.layers.start;
//
//	while (item2 != NULL)
//	{
//		RELEASE(item2->data);
//		item2 = item2->next;
//	}
//	data.layers.clear();
//
//	// Clean up the pugui tree
//	mapFile.reset();
//
//    return true;
//}
//
//// Load new map
//bool Map::Load(const char* filename)
//{
//    bool ret = true;
//    SString tmp("%s%s", folder.GetString(), filename);
//
//    pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());
//
//    if(result == NULL)
//    {
//        LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
//        ret = false;
//    }
//
//	// Load general info
//    if(ret == true)
//    {
//        // L03: DONE 3: Create and call a private function to load and fill all your map data
//		ret = LoadMap();
//	}
//
//    // L03: DONE 4: Create and call a private function to load a tileset
//    // remember to support more any number of tilesets!
//	pugi::xml_node tileset;
//	for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
//	{
//		TileSet* set = new TileSet();
//
//		if (ret == true) ret = LoadTilesetDetails(tileset, set);
//
//		if (ret == true) ret = LoadTilesetImage(tileset, set);
//
//		data.tilesets.add(set);
//	}
//
//	// L04: DONE 4: Iterate all layers and load each of them
//	// Load layer info
//	pugi::xml_node layer;
//	for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
//	{
//		MapLayer* lay = new MapLayer();
//
//		ret = LoadLayer(layer, lay);
//
//		if (ret == true)
//			data.layers.add(lay);
//	}
//    
//    if(ret == true)
//    {
//        // L03: DONE 5: LOG all the data loaded iterate all tilesets and LOG everything
//		LOG("Successfully parsed map XML file: %s\n", filename);
//		LOG("Width:	%d		Height: %d\n", data.width, data.height);
//		LOG("Tile_Width: %d	Tile_Height: %d\n", data.tileWidth, data.tileHeight);
//		LOG("Tileset: ----\n");
//		LOG("Name: %s		Firstgid: %d\n", mapFile.child("map").child("tileset").attribute("name").as_string(), mapFile.child("map").child("tileset").attribute("firstgid").as_int());
//		LOG("Tile Width: %d	Tile Height: %d\n", mapFile.child("map").child("tileset").attribute("tilewidth").as_int(), mapFile.child("map").child("tileset").attribute("tileheight").as_int());
//		LOG("Spacing: %d	Margin: %d\n", mapFile.child("map").child("tileset").attribute("spacing").as_int(), mapFile.child("map").child("tileset").attribute("margin").as_int());
//
//
//		// L04: DONE 4: LOG the info for each loaded layer
//		for (int i = 0; i < data.layers.count(); i++)
//		{
//		LOG("Layer ----\n");
//		LOG("Name: %s\n", data.layers.At(i)->data->name.GetString());
//		LOG("Tile Width: %d	Tile Height: %d\n", data.layers.At(i)->data->width, data.layers.At(i)->data->height);
//		}
//    }
//
//    mapLoaded = ret;
//
//    return ret;
//}
//
//// L03: DONE: Load map general properties
//bool Map::LoadMap()
//{
//	bool ret = true;
//	pugi::xml_node map = mapFile.child("map");
//
//	if (map == NULL)
//	{
//		LOG("Error parsing map xml file: Cannot find 'map' tag.");
//		ret = false;
//	}
//	else
//	{
//		// L03: DONE: Load map general properties
//		data.width = map.attribute("width").as_int();
//		data.height = map.attribute("height").as_int();
//		data.tileWidth = map.attribute("tilewidth").as_int();
//		data.tileHeight = map.attribute("tileheight").as_int();
//		if (strcmp(map.attribute("orientation").as_string("MAPTYPE_UNKNOWN"), "orthogonal") == 0)
//			data.type = MAPTYPE_ORTHOGONAL;
//		else if (strcmp(map.attribute("orientation").as_string("MAPTYPE_UNKNOWN"), "isometric") == 0)
//			data.type = MAPTYPE_ISOMETRIC;
//		else if (strcmp(map.attribute("orientation").as_string("MAPTYPE_UNKNOWN"), "staggered") == 0)
//			data.type = MAPTYPE_STAGGERED;
//	}
//
//	return ret;
//}
//
//// L03: DONE: Load Tileset attributes
//bool Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
//{
//	bool ret = true;
//	
//	// L03: DONE: Load Tileset attributes
//	set->name = tileset_node.attribute("name").as_string();
//	set->firstgid = tileset_node.attribute("firstgid").as_int();
//	set->tileWidth = tileset_node.attribute("tilewidth").as_int(0);	
//	set->tileHeight = tileset_node.attribute("tileheight").as_int(0);
//	set->margin = tileset_node.attribute("margin").as_int();
//	set->spacing = tileset_node.attribute("spacing").as_int();
//	set->tileWidth = tileset_node.attribute("tilewidth").as_int();
//	set->tileHeight = tileset_node.attribute("tileheight").as_int();
//	set->tileCount = tileset_node.attribute("tilecount").as_int(0);
//	return ret;
//}
//
//// L03: DONE: Load Tileset image
//bool Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
//{
//	bool ret = true;
//	pugi::xml_node image = tileset_node.child("image");
//
//	if (image == NULL)
//	{
//		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
//		ret = false;
//	}
//	else
//	{
//		// L03: DONE: Load Tileset image
//		set->texture = app->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
//		set->texWidth = image.attribute("width").as_int();
//		set->texHeight = image.attribute("height").as_int();
//		set->numTilesWidth = set->texWidth / set->tileWidth;
//		set->numTilesHeight = set->texHeight / set->tileHeight;
//		set->offsetX = 0;
//		set->offsetY = 0;
//	}
//
//	return ret;
//}
//
//// L04: DONE 3: Create the definition for a function that loads a single layer
//bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
//{
//	bool ret = true;
//	
//	// L04: DONE 3: Load a single layer
//	layer->name = node.attribute("name").as_string("");
//	layer->width = node.attribute("width").as_int(0);
//	layer->height = node.attribute("height").as_int(0);
//
//	int size = layer->width * layer->height;
//
//	uint* pointer = new uint[size];
//	memset(pointer, 0, size);
//	layer->data = pointer;
//	if (layer->data == NULL) {
//		LOG("Error al crear el array de layer");
//		ret = false;
//	}
//	else
//	{
//		pugi::xml_node tileNode = node.child("data").child("tile");
//		int i = 0;
//		for (pugi::xml_node tile = tileNode; tile && ret; tile = tile.next_sibling("tile"))
//		{
//			layer->data[i] = tile.attribute("gid").as_uint(0);
//			i++;
//		}
//		i = 0;
//
//	}
//
//	return ret;
//}
//
//
//// L06: TODO 6: Load a group of properties from a node and fill a list with it
//bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
//{
//	bool ret = false;
//	pugi::xml_node propertyNode = node.child("property");
//
//
//	for (propertyNode; propertyNode && ret; propertyNode = propertyNode.next_sibling("property"))
//	{
//		Properties::Property* propertyID = new Properties::Property();
//		propertyID->name = propertyNode.attribute("name").as_string("");
//		propertyID->value = propertyNode.attribute("value").as_int(-1);
//		properties.list.add(propertyID);
//	}
//
//	return ret;
//}
