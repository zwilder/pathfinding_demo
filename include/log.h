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
#ifndef LOG_H
#define LOG_H

void write_htable_hr(Vec2iHT *table);
void write_htable_csv(Vec2iHT *table, Vec2i start, Vec2i goal);
void write_vlist_csv(Vec2iList *list,Vec2i start, Vec2i goal);
void write_dijkstra_map(Vec2iHT *map, Vec2i start);
void write_explored_map(Vec2iHT *map, Vec2i start, Vec2i goal);
#endif
