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
#ifndef RECT_H
#define RECT_H

typedef struct {
    Vec2i dim;
    Vec2i pos;
} Rect;

struct RectList {
    Rect data;
    struct RectList *next;
};

typedef struct RectList RectList;

/****************
 * Rect functions
 ****************/
Rect make_rect(int x, int y, int width, int height); 
bool point_in_rect(Rect a, Vec2i b);
Vec2i random_point_in_rect(Rect a);
bool rect_intersect(Rect a, Rect b);
Vec2i get_center(Rect a);

/********************
 * RectList functions
 ********************/
RectList* create_RectList(Rect data);
void push_RectList(RectList **headref, Rect data);
Rect pop_RectList(RectList **headref);
int count_RectList(RectList *headref);
void destroy_RectList(RectList **headref);

#endif
