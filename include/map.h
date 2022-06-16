/*
* Pathfinding Demo
* Copyright (C) Zach Wilder 2022
* 
* This file is a part of Pathfinding Demo
*
* Pathfinding Demo is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* Pathfinding Demo is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with Pathfinding Demo.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef MAP_H
#define MAP_H

typedef struct {
    char ch;
    bool blksmv;
    int fg;
    int bg;
} Tile;

typedef enum {
    T_EMPTY = 0,
    T_FLOOR,
    T_WALL,
    T_UP,
    T_DN,
    NUM_TILES
} TileType;

extern const int MAP_WIDTH;
extern const int MAP_HEIGHT;
extern const int MIN_ROOM_SIZE;
extern const int MAX_ROOM_SIZE;
extern const int MIN_NUM_ROOMS;
extern const int MAX_NUM_ROOMS;
extern Tile **g_map;

void place_tile_at(Vec2i pos,int type); 
Tile** generate_map(void);
Vec2i build_dungeon(void);
void place_room(Rect room);
void place_corridor(Vec2i a, Vec2i b);
void place_htunnel(int x1, int x2, int y);
void place_vtunnel(int y1, int y2, int x);
void destroy_map(void);
bool is_blocked(Vec2i pos);
bool in_bounds(Vec2i pos);
Vec2i get_stair_pos(void);

#endif
