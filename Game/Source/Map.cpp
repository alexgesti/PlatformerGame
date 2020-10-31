
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

// L06: TODO 7: Ask for the value of a custom property
int Properties::GetProperty(const char* value, int defaultValue) const
{
	for (int i = 0; i < list.count(); i++)
	{
		if (strcmp(list.At(i)->data->name.GetString(), value) == 0)
		{
			if (list.At(i)->data->value != defaultValue)
				return list.At(i)->data->value;
		}
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

// Draw the map (all requried layers)
void Map::Draw()
{
	if (mapLoaded == false) return;

	// L04: DONE 5: Prepare the loop to draw all tilesets + DrawTexture()
	ListItem<MapLayer*>* layer = data.layers.start;

	// L06: DONE 4: Make sure we draw all the layers and not just the first one
	for (layer; layer != nullptr; layer = layer->next)
	{
		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int tileId = layer->data->Get(x, y);
				if (tileId > 0)
				{
					// L04: DONE 9: Complete the draw function
					iPoint vec = MapToWorld(x, y);
					for (int i = 0; i < data.tilesets.count(); i++)
					{
						if (showCollider == true) {
							app->render->DrawTexture(GetTilesetFromTileId(tileId)->texture, vec.x, vec.y, &GetTilesetFromTileId(tileId)->GetTileRect(tileId));
						}
						if (showCollider == false) {
							if (layer->data->properties.GetProperty("Draw", 0) == 0)
								app->render->DrawTexture(GetTilesetFromTileId(tileId)->texture, vec.x, vec.y, &GetTilesetFromTileId(tileId)->GetTileRect(tileId));
						}
					}
				}
			}
		}
	}
}

// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret(0, 0);

	// L05: DONE 1: Add isometric map to world coordinates
	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tileWidth;
		ret.y = y * data.tileHeight;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tileWidth / 2);
		ret.y = (x + y) * (data.tileHeight / 2);
	}
	else
	{
		LOG("Unknown map type(M2W)\n");
		ret.x = x; ret.y = y;
	}

	return ret;
}

// L05: DONE 2: Add orthographic world to map coordinates
iPoint Map::WorldToMap(int x, int y) const
{
	iPoint ret;

	// L05: DONE 3: Add the case for isometric maps to WorldToMap
	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tileWidth;
		ret.y = y / data.tileHeight;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{

		float half_width = data.tileWidth * 0.5f;
		float half_height = data.tileHeight * 0.5f;
		ret.x = int((x / half_width + y / half_height) / 2);
		ret.y = int((y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type (W2M)\n");
		ret.x = x; ret.y = y;
	}

	return ret;
}

// L06: DONE 3: Pick the right Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int id) const
{
	ListItem<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	for (set; set; item = item->next, set = item->data)
	{
		if (id >= set->firstgid && id < set->firstgid + set->tileCount)
			return set;
	}

	return set;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };

	// L04: DONE 7: Get relative Tile rectangle
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

	// L03: DONE 2: Make sure you clean up any memory allocated from tilesets/map
	// Remove all tilesets
	ListItem<TileSet*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// L04: DONE 2: clean up all layer data
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
		// L03: DONE 3: Create and call a private function to load and fill all your map data
		ret = LoadMap();
	}

	// L03: DONE 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!
	pugi::xml_node tileset;
	for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true) ret = LoadTilesetDetails(tileset, set);

		if (ret == true) ret = LoadTilesetImage(tileset, set);

		data.tilesets.add(set);
	}

	// L04: DONE 4: Iterate all layers and load each of them
	// Load layer info
	pugi::xml_node layer;
	for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if (ret == true)
			data.layers.add(lay);
	}

	if (ret == true)
	{
		// L03: DONE 5: LOG all the data loaded iterate all tilesets and LOG everything
		LOG("Successfully parsed map XML file: %s\n", filename);
		LOG("Width:	%d		Height: %d\n", data.width, data.height);
		LOG("Tile_Width: %d	Tile_Height: %d\n", data.tileWidth, data.tileHeight);
		LOG("Tileset: ----\n");
		LOG("Name: %s		Firstgid: %d\n", mapFile.child("map").child("tileset").attribute("name").as_string(), mapFile.child("map").child("tileset").attribute("firstgid").as_int());
		LOG("Tile Width: %d	Tile Height: %d\n", mapFile.child("map").child("tileset").attribute("tilewidth").as_int(), mapFile.child("map").child("tileset").attribute("tileheight").as_int());
		LOG("Spacing: %d	Margin: %d\n", mapFile.child("map").child("tileset").attribute("spacing").as_int(), mapFile.child("map").child("tileset").attribute("margin").as_int());


		// L04: DONE 4: LOG the info for each loaded layer
		for (int i = 0; i < data.layers.count(); i++)
		{
			LOG("Layer ----\n");
			LOG("Name: %s\n", data.layers.At(i)->data->name.GetString());
			LOG("Tile Width: %d	Tile Height: %d\n", data.layers.At(i)->data->width, data.layers.At(i)->data->height);
		}
	}

	mapLoaded = ret;

	return ret;
}

// L03: DONE: Load map general properties
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
		// L03: DONE: Load map general properties
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tileWidth = map.attribute("tilewidth").as_int();
		data.tileHeight = map.attribute("tileheight").as_int();
		//data.orientation = map.attribute("orientation").as_string();
		if (strcmp(map.attribute("orientation").as_string("MAPTYPE_UNKNOWN"), "orthogonal") == 0)
			data.type = MAPTYPE_ORTHOGONAL;
		else if (strcmp(map.attribute("orientation").as_string("MAPTYPE_UNKNOWN"), "isometric") == 0)
			data.type = MAPTYPE_ISOMETRIC;
		else if (strcmp(map.attribute("orientation").as_string("MAPTYPE_UNKNOWN"), "staggered") == 0)
			data.type = MAPTYPE_STAGGERED;
	}

	return ret;
}

// L03: DONE: Load Tileset attributes
bool Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;

	// L03: DONE: Load Tileset attributes
	set->name = tileset_node.attribute("name").as_string();
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	set->tileWidth = tileset_node.attribute("tilewidth").as_int();
	set->tileHeight = tileset_node.attribute("tileheight").as_int();
	set->tileCount = tileset_node.attribute("tilecount").as_int();
	return ret;
}

// L03: DONE: Load Tileset image
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
		// L03: DONE: Load Tileset image
		set->texture = app->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		set->texWidth = image.attribute("width").as_int();
		set->texHeight = image.attribute("height").as_int();
		set->numTilesWidth = set->texWidth / set->tileWidth;
		set->numTilesHeight = set->texHeight / set->tileHeight;
		set->offsetX = 0;
		set->offsetY = 0;
	}

	return ret;
}

// L04: DONE 3: Create the definition for a function that loads a single layer
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	// L04: DONE 3: Load a single layer
	layer->name = node.attribute("name").as_string("");
	layer->width = node.attribute("width").as_int(0);
	layer->height = node.attribute("height").as_int(0);
	LoadProperties(node, layer->properties);

	int size = layer->width * layer->height;

	uint* pointer = new uint[size];
	memset(pointer, 0, size);
	layer->data = pointer;
	if (layer->data == NULL) {
		LOG("Error al crear el array de layer");
		ret = false;
	}
	else
	{
		pugi::xml_node tileNode = node.child("data").child("tile");
		int i = 0;
		for (pugi::xml_node tile = tileNode; tile && ret; tile = tile.next_sibling("tile"))
		{
			layer->data[i] = tile.attribute("gid").as_uint(0);
			i++;
		}
		i = 0;

	}

	return ret;
}


// L06: TODO 6: Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = true;
	pugi::xml_node propertyNode = node.child("properties").child("property");


	for (propertyNode; propertyNode && ret; propertyNode = propertyNode.next_sibling("property"))
	{
		Properties::Property* propertyID = new Properties::Property();
		propertyID->name = propertyNode.attribute("name").as_string("");
		propertyID->value = propertyNode.attribute("value").as_int(-1);
		properties.list.add(propertyID);
	}

	return ret;
}
