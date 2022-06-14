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

#ifndef PATHFINDING_H
#define PATHFINDING_H

/********
 * System
 ********/
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

/******
 * Core
 ******/
#include <vec2i.h>
#include <color.h>
#include <rect.h>
#include <mt19937.h>
#include <map.h>

/**********************
 * Function Definitions
 **********************/
Vec2iList* open_neighbors_at(Vec2i pos, bool checkMonsters); 
int movement_cost_at(Vec2i pos);

Vec2iList* bh_line(Vec2i start, Vec2i finish);
void bh_line_add(Vec2iList **head, Vec2i pos); 

Vec2iList* breadth_first_search(Vec2i start, bool monsterblock); 
Vec2iHT* bfs_path(Vec2i start, Vec2i goal, bool monsterblock);
Vec2iHT* gbfs_path(Vec2i start, Vec2i goal, bool monsterblock); 
Vec2iHT* astar_path(Vec2i start, Vec2i goal, bool monsterblock); 
#endif
