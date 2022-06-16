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
#include <pathfinding.h>

const int MAP_WIDTH = 80;
const int MAP_HEIGHT = 24;
const int MIN_ROOM_SIZE = 5;
const int MAX_ROOM_SIZE = 10;
const int MIN_NUM_ROOMS = 8;
const int MAX_NUM_ROOMS = 30;
Tile **g_map = NULL;

Tile tile_table[NUM_TILES] = 
{
    {' ',true,BLACK,BLACK},
    {'.',false,WHITE,BLACK},
    {'#',true,WHITE,BLACK},
    {'<',false,WHITE,BLACK},
    {'>',false,WHITE,BLACK}
};

void place_tile_at(Vec2i pos,int type) {
    g_map[pos.x][pos.y] = tile_table[type];
}

Tile** generate_map(void) {
    Tile **map = malloc(MAP_WIDTH * sizeof(Tile*));
    int x = 0;
    int y = 0;
    for(x = 0; x < MAP_WIDTH; x++) {
        map[x] = malloc(MAP_HEIGHT * sizeof(Tile));
        for(y = 0; y < MAP_HEIGHT; y++) {
            map[x][y] = tile_table[T_EMPTY];
        }
    }
    return map;
}

Vec2i build_dungeon(void) {
    /* Build the dungeon, return the start pos */
    Vec2i result;
    int x,y,w,h;
    Rect newRoom;
    bool intersects;
    int numRooms = 0;
    int attempts = 0;
    int minattempts = 0;
    RectList *rooms = NULL;
    RectList *tmp = NULL;

    while(numRooms < MIN_NUM_ROOMS && minattempts < 100) {
        while(numRooms < MAX_NUM_ROOMS && attempts < 500) {
            x = mt_rand(1, MAP_WIDTH - MAX_ROOM_SIZE - 1);
            y = mt_rand(1, MAP_HEIGHT - MAX_ROOM_SIZE - 1);
            w = mt_rand(MIN_ROOM_SIZE, MAX_ROOM_SIZE);
            h = mt_rand(MIN_ROOM_SIZE, MAX_ROOM_SIZE);
            while(x % 2 != 0) {
                x = mt_rand(1, MAP_WIDTH - 1);
            }
            while(y % 2 != 0) {
                y = mt_rand(1, MAP_HEIGHT - 1);
            }
            newRoom = make_rect(x,y,w,h);
            intersects = false;
            tmp = rooms;
            while(tmp) {
                if(rect_intersect(newRoom, tmp->data)) {
                    intersects = true;
                    break;
                }
                tmp = tmp->next;
            }
            if((newRoom.pos.x + newRoom.dim.x >= MAP_WIDTH) ||
                   (newRoom.pos.y + newRoom.dim.y >= MAP_HEIGHT)) {
                intersects = true;
            }
            if(!intersects) {
                place_room(newRoom);
                push_RectList(&rooms, newRoom);
                numRooms = count_RectList(rooms);
            }
            attempts++;
        }
        minattempts++;
    }

    tmp = rooms;
    while(tmp->next) {
        place_corridor(get_center(tmp->data), get_center(tmp->next->data));
        tmp = tmp->next;
    }
    
    place_tile_at(get_center(tmp->data), T_DN);
    result = get_center(rooms->data);
    destroy_RectList(&rooms);
    return result;
}

void place_room(Rect room) {
    int x, y;
    for(x = room.pos.x; x <= room.pos.x + room.dim.x; x++){
        for(y=room.pos.y; y <= room.pos.y + room.dim.y; y++) {
            if(room.pos.x == x 
                   || room.pos.y == y
                   || room.dim.x + room.pos.x == x 
                   || room.dim.y + room.pos.y == y) {
                place_tile_at(make_vec(x,y), T_WALL);
            } else {
                place_tile_at(make_vec(x,y), T_FLOOR);
            }
        }
    }
}

void place_corridor(Vec2i a, Vec2i b) {
    if(mt_bool()) {
        place_htunnel(b.x, a.x, b.y);
        place_vtunnel(b.y, a.y, a.x);
    } else {
        place_vtunnel(b.y, a.y, b.x);
        place_htunnel(b.x, a.x, a.y);
    } 
}

void place_htunnel(int x1, int x2, int y) {
    int i;
    int min = (x1 < x2 ? x1 : x2);
    int max = (x1 > x2 ? x1 : x2);
    for(i = min; i <= max; ++i)
    {
        place_tile_at(make_vec(i,y), T_FLOOR);
        if(g_map[i][y-1].ch == ' ') {
            place_tile_at(make_vec(i,y-1), T_WALL);
        }
        if(g_map[i][y+1].ch == ' ') {
            place_tile_at(make_vec(i,y+1), T_WALL);
        }
    }
}

void place_vtunnel(int y1, int y2, int x) {
    int i;
    int min = (y1 < y2 ? y1 : y2);
    int max = (y1 > y2 ? y1 : y2);
    for(i = min; i <= max; ++i)
    {
        place_tile_at(make_vec(x,i), T_FLOOR);
        if(g_map[x-1][i].ch == ' ') {
            place_tile_at(make_vec(x-1,i), T_WALL);
        }
        if(g_map[x+1][i].ch == ' ') {
            place_tile_at(make_vec(x+1,i), T_WALL);
        }
    }
}

void destroy_map(void) {
    int x;
    for(x = 0; x < MAP_WIDTH; x++) {
        free(g_map[x]);
    }
    free(g_map);
}

bool is_blocked(Vec2i pos) {
    if(!in_bounds(pos)) {
        return true;
    }
    return(g_map[pos.x][pos.y].blksmv);
}

bool in_bounds(Vec2i pos) {
    if((pos.x < 0) || (pos.x > MAP_WIDTH)) {
        return false; 
    }
    if((pos.y < 0) || (pos.y > MAP_HEIGHT)) {
        return false;
    }
    return true;
}

Vec2i get_stair_pos(void) {
    Vec2i result;
    int x,y;
    for(x = 0; x < MAP_WIDTH; x++) {
        for(y = 0; y < MAP_HEIGHT; y++) {
            if(g_map[x][y].ch == '>') {
                result = make_vec(x,y);
                break;
            }
        }
    }
    return result;
}
