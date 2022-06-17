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

void write_htable_hr(Vec2iHT *table) {
    int i = 0;
    Vec2iHTList *head = NULL;
    FILE *fp = fopen("scratch_log.txt","w+");
    fprintf(fp,"\n\t********************\n");
    fprintf(fp,"\tHashTable\n\t********************\n");
    for(i = 0; i < table->size; i++) {
        if(table->items[i]) {
            fprintf(fp,"Index: %d | Key: [%d,%d] | Value: [%d,%d]\n",
                    i, table->items[i]->key.x,table->items[i]->key.y,
                    table->items[i]->value.x,table->items[i]->value.y);
            if(table->ofbuckets[i]) {
                fprintf(fp,"\t=> Overflow Bucket =>\n");
                head = table->ofbuckets[i];
                while(head) {
                    fprintf(fp,"\tKey: [%d,%d] | Value: [%d,%d]\n",
                            head->item->key.x,head->item->key.y,
                            head->item->value.x,head->item->value.y);
                    head = head->next;
                }
            }
        }
    }
    printf("\t********************\n\n");
    fclose(fp);
}

void write_htable_csv(Vec2iHT *table, Vec2i start, Vec2i goal) {
    int i = 0;
    Vec2iHTList *head = NULL;
    FILE *fp = fopen("scratch_log.csv","w+");
    fprintf(fp,",,,,,,sx,sy,gx,gy\n");
    fprintf(fp,",,,,,,%d,%d,%d,%d\n",start.x,start.y,goal.x,goal.y);
    fprintf(fp,"index,kx,ky,vx,vy,ofb\n");
    for(i = 0; i < table->size; i++) {
        if(table->items[i]) {
            fprintf(fp,"%d,%d,%d,%d,%d,0\n",
                    i, table->items[i]->key.x,table->items[i]->key.y,
                    table->items[i]->value.x,table->items[i]->value.y);
            if(table->ofbuckets[i]) {
                head = table->ofbuckets[i];
                while(head) {
                    fprintf(fp,",%d,%d,%d,%d,1\n",
                            head->item->key.x,head->item->key.y,
                            head->item->value.x,head->item->value.y);
                    head = head->next;
                }
            }
        }
    }
    fclose(fp);
}

void write_vlist_csv(Vec2iList *list,Vec2i start, Vec2i goal) {
    Vec2iList *tmp = list;
    FILE *fp = fopen("scratch_vlist_log.csv", "w+");
    fprintf(fp,",,sx,sy,gx,gy\n");
    fprintf(fp,",,%d,%d,%d,%d\n",start.x,start.y,goal.x,goal.y);
    fprintf(fp,"x,y\n");
    while(tmp) {
        fprintf(fp,"%d,%d\n",tmp->item.x,tmp->item.y);
        tmp = tmp->next;
    }
    fclose(fp);
}

void write_dijkstra_map(Vec2iHT *map, Vec2i start) {
    FILE *fp = fopen("scratch_dijkstra.txt", "w");
    int x,y;
    Vec2i pos;
    Vec2i cost;
    fprintf(fp,"Dijkstra Map:\n   ");
    for(x = 0; x < MAP_WIDTH; x++) {
        if(x < 10) {
            fprintf(fp, " %d ", x);
        } else {
            fprintf(fp, " %d", x);
        }
    }
    fprintf(fp, "\n");
    for(y = 0; y < MAP_HEIGHT; y++) {
        if(y < 10) {
            fprintf(fp, "%d : ", y);
        } else {
            fprintf(fp, "%d: ", y);
        }
        for(x = 0; x < MAP_WIDTH; x++) {
            pos = make_vec(x,y);
            if(eq_vec(pos, start)) {
                fprintf(fp, "@  ");
                continue;
            }
            cost = search_Vec2iHT(map, pos);
            if(!vec_null(cost)) {
                if(cost.x < 10 ) {
                    fprintf(fp, "%d  ", cost.x);
                } else if (cost.x < 100) {
                    fprintf(fp, "%d ", cost.x);
                } else {
                    fprintf(fp, ".  ");
                }
            } else {
                fprintf(fp," _ ");
            }
        }
        fprintf(fp,"\n");
    }
}

void write_explored_map(Vec2iHT *map, Vec2i start, Vec2i goal) {
    FILE *fp = fopen("scratch_explored.txt", "w");
    int x,y;
    Vec2i pos;
    Vec2i explored;
    fprintf(fp,"Explored Map:\n   ");
    for(x = 0; x < MAP_WIDTH; x++) {
        if(x < 10) {
            fprintf(fp, " %d ", x);
        } else {
            fprintf(fp, " %d", x);
        }
    }
    fprintf(fp, "\n");
    for(y = 0; y < MAP_HEIGHT; y++) {
        if(y < 10) {
            fprintf(fp, "%d : ", y);
        } else {
            fprintf(fp, "%d: ", y);
        }
        for(x = 0; x < MAP_WIDTH; x++) {
            pos = make_vec(x,y);
            if(eq_vec(pos, start)) {
                fprintf(fp, "@  ");
                continue;
            }
            if(eq_vec(pos, goal)) {
                fprintf(fp, ">  ");
                continue;
            }
            explored = search_Vec2iHT(map, pos);
            if(!vec_null(explored)) {
                fprintf(fp, "x  ");
                /*
                if(cost.x < 10 ) {
                    fprintf(fp, "%d  ", cost.x);
                } else if (cost.x < 100) {
                    fprintf(fp, "%d ", cost.x);
                } else {
                    fprintf(fp, ".  ");
                }
                */
            } else {
                fprintf(fp," _ ");
            }
        }
        fprintf(fp,"\n");
    }
}
