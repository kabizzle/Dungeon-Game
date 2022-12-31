/* mapgen.c -- Generates the game map
 * 
 * dungeon creation algorithm ported (and modified) from
 * http://gamedevelopment.tutsplus.com/tutorials/create-a-procedurally-generated-dungeon-cave-system--gamedev-10099
 */

#include "dungeon.h"
#include <stdlib.h>

/**
 * \brief Initialize a new room, with given coordinates (x,y) and dimensions (w,h)
 * 
 * \param nr The room to be initialized
 * \param x Center x-coordinate of the room
 * \param y Center y-coordinate of the room
 * \param w Width of the room
 * \param h Height of the room
 */
void newRoom(Room* nr, int x, int y, unsigned int w, unsigned int h) {
	nr->x1 = x;
	nr->x2 = x + w;
	nr->y1 = y;
	nr->y2 = y + h;
	nr->center.x = (nr->x1 + nr->x2) / 2;
	nr->center.y = (nr->y1 + nr->y2) / 2;
}

/**
 * \brief Checks whether two rooms intersect
 * 
 * \param a The first room
 * \param b The second room
 * \returns 1 if the rooms overlap
 * \returns 0 if they do not. 
 */
int intersects(const Room* a, const Room* b) {
	return (a->x1 <= b->x2 && a->x2 >= b->x1 && a->y1 <= b->y2 &&
			a->y2 >= b->y2);
}

/**
 * \brief Sets the map tiles within a room to ROOM tile
 * 
 * \param nr The room 
 * \param m The map
 */
void initRoom(Room* nr, Map* m) {
	for(int i = nr->y1; i < nr->y2; i++) {
		for(int j = nr->x1; j < nr->x2; j++) {
			m->tile[i][j] = TILE_ROOM;
		}
	}
}

/**
 * \brief Sets a a horizontal corridor at given coordinates starting from 
 *        the lower y-coordinate till the higher y-coordinate. 
 * 
 * \details A horizontal corridor is composed of OPEN tiles on a horizontal line 
 * 
 * \param map The map of the game
 * \param x1 The first horizontal coordinate
 * \param x2 The second horizontal coordinate
 * \param y The vertical coordinate of the horizontal corridor
 */
void hCorridor(Map* map, int x1, int x2, int y) {
	int minx = (x1 < x2 ? x1 : x2);
	int maxx = (x1 < x2 ? x2 : x1);

	for(int x = minx; x <= maxx; x++) {
		map->tile[y][x] = TILE_OPEN;
	}
}

/**
 * \brief Sets a a vertical corridor at given coordinates starting from 
 *        the lower y-coordinate till the higher y-coordinate. 
 * 
 * \details A vertical corridor is composed of OPEN tiles on a vertical line 
 * 
 * \param map The map of the game
 * \param y1 The first vertical coordinate
 * \param y2 The second vertical coordinate
 * \param x The horizontal coordinate of the vertical corridor
 */
void vCorridor(Map* map, int y1, int y2, int x) {
	int miny = (y1 < y2 ? y1 : y2);
	int maxy = (y1 < y2 ? y2 : y1);

	for(int y = miny; y <= maxy; y++) {
		map->tile[y][x] = TILE_OPEN;
	}
}

/**
 * \brief Draw corridors between two rooms by randomly placing vertical 
 * or horizontal corridor.
 * 
 * \param m The map of the game
 * \param a The first room
 * \param b The second room
 */
void drawCorridors(Map* m, Room* a, Room* b) {
	if(rand() & 1) {
		hCorridor(m, a->center.x, b->center.x, a->center.y);
		vCorridor(m, a->center.y, b->center.y, b->center.x);
	} else {
		vCorridor(m, a->center.y, b->center.y, a->center.x);
		hCorridor(m, a->center.x, b->center.x, b->center.y);
	}
}

/**
 * \brief Places rooms on the map and create corridors between them
 * 
 * \param game The game
 */
void placeRooms(Game* game) {
	Map* map = &game->map;
	Options* opt = &game->opts;
	Room* rooms;
	rooms = malloc(sizeof(Room) * opt->numRooms);
	if(!rooms) {
		// No use going on
		exit(EXIT_FAILURE);
	}

	int failed = 1;
	for(unsigned int i = 0; i < opt->numRooms; i++) {
		do {
			unsigned int w = opt->minRoomSize +
							 rand() % (opt->maxRoomSize - opt->minRoomSize);
			unsigned int h = opt->minRoomSize +
							 rand() % (opt->maxRoomSize - opt->minRoomSize);
			int x = rand() % (opt->mapWidth - w - 1) + 1;
			int y = rand() % (opt->mapHeight - h - 1) + 1;
			newRoom(&rooms[i], x, y, w, h);

			failed = 0;
			for(unsigned int j = 0; j < i; j++) {
				// if overlaps with another room, have to pick another location
				if(intersects(&rooms[i], &rooms[j])) {
					failed = 1;
					break;
				}
			}
			if(!failed) {
				initRoom(&rooms[i], map);
				if(i > 0)
					drawCorridors(map, &rooms[i - 1], &rooms[i]);
			}
		} while(failed);
	}
	// Player is initially placed in the center of first room
	game->position = rooms[0].center;
	free(rooms); // not needed anymore
}

/** 
 * \brief Generates game map by first filling everything as wall, then
 *        creates rooms and corridors using placeRooms function
 * 
 * \param game The game
 */
void generateMap(Game* game) {
	Map* map = &game->map;
	Options* opt = &game->opts;

	map->tile = malloc(sizeof(Tile*) * opt->mapHeight);
	for(unsigned int i = 0; i < opt->mapHeight; i++) {
		map->tile[i] = malloc(sizeof(Tile) * opt->mapWidth);
		for(unsigned int j = 0; j < opt->mapWidth; j++) {
			map->tile[i][j] = TILE_WALL;
		}
	}
	placeRooms(game);
}

/**
 * \brief Frees all dynamically allocated memory
 * 
 * \param game The game
 */
void freeAll(Game* game) {
	if(game->map.tile) {
		for(unsigned int i = 0; i < game->opts.mapHeight; i++) {
			free(game->map.tile[i]);
		}
		free(game->map.tile);
	}
	if(game->monsters)
		free(game->monsters);
}
