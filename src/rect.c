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

/****************
 * Rect functions
 ****************/
Rect make_rect(int x, int y, int width, int height) {
    Rect result;
    result.pos.x = x;
    result.pos.y = y;
    result.dim.x = width;
    result.dim.y = height;
    return result;
}

bool point_in_rect(Rect a, Vec2i b) {
    bool x = ((b.x > a.pos.x) && (b.x < a.dim.x));
    bool y = ((b.y > a.pos.y) && (b.y < a.dim.y));
    return (x && y);
}

Vec2i random_point_in_rect(Rect a) {
    Vec2i result = make_vec(0,0);
    result.x = mt_rand(a.pos.x + 1, a.pos.x + a.dim.x - 1);
    result.y = mt_rand(a.pos.y + 1, a.pos.y + a.dim.y - 1);
    return result;
}

bool rect_intersect(Rect a, Rect b) {
    /* https://silentmatt.com/rectangle-intersection/ */
    int ax1,ay1,ax2,ay2;
    int bx1,by1,bx2,by2;
    bool result = false;

    ax1 = a.pos.x;
    ay1 = a.pos.y;
    ax2 = ax1 + a.dim.x;
    ay2 = ay1 + a.dim.y;
    bx1 = b.pos.x;
    by1 = b.pos.y;
    bx2 = bx1 + b.dim.x;
    by2 = by1 + b.dim.y;
    
    if((ax1 <= bx2) && (ax2 >= bx1) && (ay1 <= by2) && (ay2 >= by1)) {
        result = true;
    }

    return result; 
}

Vec2i get_center(Rect a) {
    Vec2i result;
    result.x = a.pos.x + (int)(a.dim.x / 2);
    result.y = a.pos.y + (int)(a.dim.y / 2);
    return result;
}

/********************
 * RectList functions
 ********************/
RectList* create_RectList(Rect data) {
    RectList *node = malloc(sizeof(RectList));
    node->data = data;
    node->next = NULL;
    return node;
}

void push_RectList(RectList **headref, Rect data) {
    RectList *node = create_RectList(data);
    if(!(*headref)) {
        *headref = node;
        return;
    }
    node->next = *headref;
    *headref = node;
}

Rect pop_RectList(RectList **headref) {
    if(!(*headref)) {
        return make_rect(0,0,0,0);
    }
    Rect data = (*headref)->data;
    RectList *tmp = *headref;
    *headref = (*headref)->next;
    free(tmp);
    return data;
}

int count_RectList(RectList *headref) {
    if(!headref) {
        return 0;
    }
    return (count_RectList(headref->next) + 1);
}

void destroy_RectList(RectList **headref) {
    if(!(*headref)) {
        return;
    }
    RectList *tmp = NULL;
    while(*headref) {
        tmp = *headref;
        *headref = (*headref)->next;
        free(tmp);
    }
}
