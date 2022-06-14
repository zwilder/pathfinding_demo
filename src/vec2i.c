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

const Vec2i NULLVEC = {INT_MIN, INT_MIN};

Vec2i make_vec(int x, int y) {
    Vec2i result = {};
    result.x = x;
    result.y = y;
    return result;
}
Vec2i add_vec(Vec2i a, Vec2i b) {
    Vec2i result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

Vec2i subtract_vec(Vec2i a, Vec2i b) {
    Vec2i result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

bool eq_vec(Vec2i a, Vec2i b) {
    return ((a.x == b.x) && (a.y == b.y));
}

bool vec_null(Vec2i a) {
    return (eq_vec(NULLVEC, a));
}

int man_dist(Vec2i a, Vec2i b) {
    return (abs(a.x - b.x) + abs(a.y - b.y));
}

/***********
 * Vec2iList
 ***********/

Vec2iList* create_Vec2i_list(Vec2i pos) {
    Vec2iList *newnode = malloc(sizeof(Vec2iList));
    newnode->item = pos;
    newnode->next = NULL;
    return newnode;
}

void push_Vec2i_list(Vec2iList **head, Vec2i pos) {
    if(!(*head)) {
        *head = create_Vec2i_list(pos);
        return;
    }
    Vec2iList *newnode = create_Vec2i_list(pos);
    newnode->next = *head;
    *head = newnode;
}

Vec2i pop_Vec2i_list(Vec2iList **head) {
    if(!(*head)) {
        return (NULLVEC);
    }
    Vec2iList *tmp = *head;
    *head = (*head)->next;
    Vec2i result = tmp->item;
    free(tmp);
    return result;
}

int count_Vec2i_list(Vec2iList *head) {
    if(!head) {
        return 0;
    }
    return (count_Vec2i_list(head->next) + 1);
}

void destroy_Vec2i_list(Vec2iList **head) {
    if(!(*head)) {
        return;
    }
    Vec2iList *tmp = NULL;
    while(*head) {
        tmp = *head;
        *head = (*head)->next;
        free(tmp);
    }
}

bool Vec2i_list_contains(Vec2iList *head, Vec2i pos) {
    bool result = false;
    Vec2iList *tmp = head;
    while(tmp) {
        if(eq_vec(tmp->item, pos)) {
            result = true;
            break;
        }
        tmp = tmp->next;
    }
    return result;
}

/*********
 * Vec2iPQ
 *********/
Vec2iPQ* create_Vec2iPQ(Vec2i item, int p) {
    Vec2iPQ *newnode = malloc(sizeof(Vec2iPQ));
    newnode->item = item;
    newnode->p = p;
    newnode->next = NULL;
    return newnode;
}

Vec2i peek_Vec2iPQ(Vec2iPQ **head) {
    return((*head)->item);
}

Vec2i pop_Vec2iPQ(Vec2iPQ **head) {
    if(!(*head)) {
        return NULLVEC;
    }
    Vec2iPQ *tmp = *head;
    Vec2i result = tmp->item;
    *head = (*head)->next;
    free(tmp);
    return result;
}

void push_Vec2iPQ(Vec2iPQ **head, Vec2i item, int p) {
    if(!(*head)) {
        *head = create_Vec2iPQ(item, p);
        return;
    }
    Vec2iPQ *start = *head;
    Vec2iPQ *newnode = create_Vec2iPQ(item, p);

    if((*head)->p > p) {
        newnode->next = *head;
        *head = newnode;
    } else {
        while(start->next && (start->next->p < p)) {
            start = start->next;
        }
        newnode->next = start->next;
        start->next = newnode;
    }
}

void destroy_Vec2iPQ(Vec2iPQ **head) {
    if(!(*head)) {
        return;
    }
    Vec2iPQ *tmp = NULL;
    while(*head) {
        tmp = *head;
        *head = (*head)->next;
        free(tmp);
    }
}

/*********
 * Vec2iHT
 *********/
unsigned long Vec2i_hash(Vec2i key, int size) {
    return(((key.y << 16) ^ key.x) % size);
}

Vec2iHTItem* create_Vec2iHTItem(Vec2i key, Vec2i value) {
    Vec2iHTItem *item = malloc(sizeof(Vec2iHTItem));
    item->key = key;
    item->value = value;
    return item;
}

Vec2iHT* create_Vec2iHT(int size) {
    int i = 0;
    Vec2iHT *table = malloc(sizeof(Vec2iHT));
    table->size = size;
    table->count = 0;
    table->items = calloc(table->size, sizeof(Vec2iHTItem));
    for(i = 0; i < table->size; i++) {
        table->items[i] = NULL;
    }
    table->ofbuckets = create_Vec2iHT_ofbuckets(table);
    return table;
}

void destroy_Vec2iHTItem(Vec2iHTItem *item) {
    if(!item) {
        return;
    }
    free(item);
}

void destroy_Vec2iHT(Vec2iHT *table) {
    int i = 0;
    Vec2iHTItem *item = NULL;
    for(i = 0; i < table->size; i++) {
        item = table->items[i];
        if(item) {
            destroy_Vec2iHTItem(item);
        }
    }
    destroy_Vec2iHT_ofbuckets(table);
    free(table->items);
    free(table);
}

void handle_Vec2iHT_collision(Vec2iHT *table, 
        unsigned long index, Vec2iHTItem *item) {
    Vec2iHTList *head = table->ofbuckets[index];
    if(!head) {
        /* Create the list */
        head = create_Vec2iHTList(item);
        table->ofbuckets[index] = head;
    } else {
        table->ofbuckets[index] = insert_Vec2iHTList(head, item);
    }
}

void insert_Vec2iHT(Vec2iHT *table, Vec2i key, Vec2i value) {
    Vec2iHTItem *item = create_Vec2iHTItem(key,value);
    int index = Vec2i_hash(key,table->size);
    Vec2iHTItem *cur = table->items[index];

    if(!cur) {
        /* Key does not exist */
        if(table->count == table->size) {
            /* Hash table full */
            destroy_Vec2iHTItem(item);
            return;
        }
        table->items[index] = item;
        table->count += 1;
    } else {
        /* Key exists */
        if(eq_vec(cur->key, key)) {
            /* Same key, update value */
            table->items[index]->value = value;
        } else {
            handle_Vec2iHT_collision(table, index, item);
        }
    }
}

Vec2i search_Vec2iHT(Vec2iHT *table, Vec2i key) {
    int index = Vec2i_hash(key, table->size);
    Vec2iHTItem *item = table->items[index];
    Vec2iHTList *head = table->ofbuckets[index];
    while(item) {
        if(eq_vec(item->key, key)) {
            return item->value;
        }
        if(!head) {
            return NULLVEC;
        }
        item = head->item;
        head = head->next;
    }
    return NULLVEC;
}

Vec2iHTList** create_Vec2iHT_ofbuckets(Vec2iHT *table) {
    Vec2iHTList **buckets = calloc(table->size, sizeof(Vec2iHTList*));
    int i;
    for(i = 0; i < table->size; i++) {
        buckets[i] = NULL;
    }
    return buckets;
}

void destroy_Vec2iHT_ofbuckets(Vec2iHT *table) {
    Vec2iHTList **buckets = table->ofbuckets;
    int i;
    for(i = 0; i < table->size; i++) {
        destroy_Vec2iHTList(buckets[i]);
    }
    free(buckets);
}

void delete_Vec2iHT(Vec2iHT *table, Vec2i key) {
    int index = Vec2i_hash(key, table->size);
    Vec2iHTItem *item = table->items[index];
    Vec2iHTList *head = table->ofbuckets[index];
    Vec2iHTList *node = NULL;
    Vec2iHTList *cur = NULL;
    Vec2iHTList *prev = NULL;

    if(!item) {
        /*Item doesn't exist */
        return;
    }
    if(!head && eq_vec(item->key, key)) {
        /* No collision chain, remove item set table index to NULL */
        table->items[index] = NULL;
        destroy_Vec2iHTItem(item);
        table->count--;
        return;
    } else if (head) {
        /* Collision chain exists */
        if(eq_vec(item->key, key)) {
            /* Remove this item and set head of list as the new item */
            destroy_Vec2iHTItem(item);
            node = head;
            head = head->next;
            node->next = NULL;
            table->items[index] = create_Vec2iHTItem(node->item->key, 
                    node->item->value);
            destroy_Vec2iHTList(node);
            table->ofbuckets[index] = head;
            return;
        }
        cur = head;
        prev = NULL;
        while(cur) {
            if(eq_vec(cur->item->key, key)) {
                if(!prev) {
                    /* First element of chain, remove chain */
                    destroy_Vec2iHTList(head);
                    table->ofbuckets[index] = NULL;
                    return;
                } else {
                    /* Somewhere else in chain */
                    prev->next = cur->next;
                    cur->next = NULL;
                    destroy_Vec2iHTList(cur);
                    table->ofbuckets[index] = head;
                    return;
                }
            }
            cur = cur->next;
            prev = cur;
        }
    }
}

/*************
 * Vec2iHTList
 *************/
Vec2iHTList* create_Vec2iHTList(Vec2iHTItem *item) {
    Vec2iHTList *list = malloc(sizeof(Vec2iHTList));
    if(!list) {
        return NULL;
    }
    list->item = item;
    list->next = NULL;
    return list;
}

Vec2iHTList* insert_Vec2iHTList(Vec2iHTList *headref, Vec2iHTItem *item) {
    Vec2iHTList *newnode = create_Vec2iHTList(item);
    Vec2iHTList *tmp = NULL;
    if(!headref) {
        headref = newnode;
        return headref;
    }else if (!headref->next) {
        headref->next = newnode;
        return headref;
    }
    tmp = headref;
    while(tmp->next->next) {
        /* Gets last node */
        tmp = tmp->next;
    }
    tmp->next = newnode;
    return headref;
}

Vec2iHTItem* pop_Vec2iHTList(Vec2iHTList **headref) {
    if(!(*headref)) {
        return NULL;
    }
    if(!(*headref)->next) {
        return NULL;
    }
    Vec2iHTList *node = (*headref)->next;
    Vec2iHTList *tmp = *headref;
    Vec2iHTItem *item = NULL;
    tmp->next = NULL;
    *headref = node;
    free(tmp->item);
    free(tmp);
    return item;
}

void destroy_Vec2iHTList(Vec2iHTList *headref) {
    Vec2iHTList *tmp = headref;
    while(headref) {
        tmp = headref;
        headref = headref->next;
        free(tmp->item);
        free(tmp);
    }
}
