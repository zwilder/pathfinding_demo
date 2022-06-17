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
#ifndef VEC2I_H
#define VEC2I_H
#include <limits.h>
/* Note to self: NULLVEC = {INT_MIN,INT_MIN} */

typedef struct Vec2iList Vec2iList;
typedef struct Vec2iHTList Vec2iHTList;
typedef struct Vec2iHT Vec2iHT;
typedef struct Vec2iPQ Vec2iPQ;

typedef struct {
    int x;
    int y;
} Vec2i;

struct Vec2iList {
    Vec2i item;
    struct Vec2iList *next;
};

struct Vec2iPQ {
    Vec2i item;
    int p;
    Vec2iPQ *next;
};

typedef struct {
    Vec2i key;
    Vec2i value;
} Vec2iHTItem;

struct Vec2iHT {
    Vec2iHTItem **items;
    Vec2iHTList **ofbuckets;
    int size;
    int count;
};

struct Vec2iHTList {
    Vec2iHTItem *item;
    struct Vec2iHTList *next;
};

extern const Vec2i NULLVEC;

/*******
 * Vec2i
 *******/
Vec2i make_vec(int x, int y);
Vec2i add_vec(Vec2i a, Vec2i b);
Vec2i subtract_vec(Vec2i a, Vec2i b);
bool eq_vec(Vec2i a, Vec2i b);
bool vec_null(Vec2i a);
int man_dist(Vec2i a, Vec2i b);
int chebyshev_dist(Vec2i a, Vec2i b);
float octile_dist(Vec2i a, Vec2i b);

/***********
 * Vec2iList
 ***********/
Vec2iList* create_Vec2i_list(Vec2i pos);
void push_Vec2i_list(Vec2iList **headref, Vec2i pos);
Vec2i pop_Vec2i_list(Vec2iList **headref);
int count_Vec2i_list(Vec2iList *headref);
void destroy_Vec2i_list(Vec2iList **headref);
bool Vec2i_list_contains(Vec2iList *head, Vec2i pos);

/*********
 * Vec2iPQ
 *********/
Vec2iPQ* create_Vec2iPQ(Vec2i data, int p);
Vec2i peek_Vec2iPQ(Vec2iPQ **head);
Vec2i pop_Vec2iPQ(Vec2iPQ **head);
void push_Vec2iPQ(Vec2iPQ **head, Vec2i data, int p);
void destroy_Vec2iPQ(Vec2iPQ **head);

/*********
 * Vec2iHT
 *********/
unsigned long Vec2i_hash(Vec2i key, int size);
Vec2iHTItem* create_Vec2iHTItem(Vec2i key, Vec2i value);
Vec2iHT* create_Vec2iHT(int size);
void destroy_Vec2iHTItem(Vec2iHTItem *item);
void destroy_Vec2iHT(Vec2iHT *table);
void handle_Vec2iHT_collision(Vec2iHT *table, 
        unsigned long index, Vec2iHTItem *item);
void insert_Vec2iHT(Vec2iHT *table, Vec2i key, Vec2i value);
Vec2i search_Vec2iHT(Vec2iHT *table, Vec2i key); 
Vec2iHTList** create_Vec2iHT_ofbuckets(Vec2iHT *table);
void destroy_Vec2iHT_ofbuckets(Vec2iHT *table);
void delete_Vec2iHT(Vec2iHT *table, Vec2i key); 

/*************
 * Vec2iHTList
 *************/
Vec2iHTList* create_Vec2iHTList(Vec2iHTItem *item); 
Vec2iHTList* insert_Vec2iHTList(Vec2iHTList *headref, Vec2iHTItem *item); 
Vec2iHTItem* pop_Vec2iHTList(Vec2iHTList **headref); 
void destroy_Vec2iHTList(Vec2iHTList *headref); 

#endif
