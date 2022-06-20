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

Vec2iPQ* open_neighbors_at(Vec2i pos, bool checkMonsters) {
    Vec2iPQ *results = NULL;
    int x, y;
    Vec2i tl,tr,bl,br;
    // Diagonal
    tl = make_vec(pos.x - 1, pos.y - 1);
    tr = make_vec(pos.x + 1, pos.y - 1);
    bl = make_vec(pos.x - 1, pos.y + 1);
    br = make_vec(pos.x + 1, pos.y + 1);
    if(!is_blocked(tl) && in_bounds(tl)){
        push_Vec2iPQ(&results, tl, 2);
    }
    if(!is_blocked(tr) && in_bounds(tr)){
        push_Vec2iPQ(&results, tr, 2);
    }
    if(!is_blocked(bl) && in_bounds(bl)){
        push_Vec2iPQ(&results, bl, 2);
    }
    if(!is_blocked(br) && in_bounds(br)){
        push_Vec2iPQ(&results, br, 2);
    }
               
    // Cardinal
    for(x = pos.x - 1, y = pos.y; x <= pos.x + 1; x += 2) {
        if(!is_blocked(make_vec(x,y)) && in_bounds(make_vec(x,y))) {
            push_Vec2iPQ(&results, make_vec(x,y), 1);
        }
    }
    for(y = pos.y - 1, x = pos.x; y <= pos.y + 1; y += 2) {
        if(!is_blocked(make_vec(x,y)) && in_bounds(make_vec(x,y))) {
            push_Vec2iPQ(&results, make_vec(x,y), 1);
        }
    }
    /*
    // All but center
    for(x = pos.x - 1; x <= pos.x + 1; x++) {
        for(y = pos.y - 1; y <= pos.y + 1; y++) {
            if(eq_vec(make_vec(x,y), pos)) {
                continue;
            }
            if(!is_blocked(make_vec(x,y)) && in_bounds(make_vec(x,y))) {
                push_Vec2i_list(&results, make_vec(x,y));
            }
        }
    }
    */
    return results;
}

int movement_cost_at(Vec2i pos) {
    /* This function will check tile flags, returning movment costs. For
     * instance, a closed door has a movement cost of 2, since it takes 1 turn
     * to open the door. */
    return 100;
}

int movement_cost_to(Vec2i a, Vec2i b) {
    /*
     * Looking for change in BOTH x and y when subtracting the vectors. A single
     * diagonal step will always end up as (-1,-1) (-1,1),(1,1), (1,-1)
     */
    Vec2i sub = subtract_vec(a,b);
    static Vec2i tl = {-1,-1};
    static Vec2i tr = {1,-1};
    static Vec2i bl = {-1,1};
    static Vec2i br = {1,1};
    if (eq_vec(sub,tl) || eq_vec(sub,tr) ||
            eq_vec(sub,bl) || eq_vec(sub,br)) {
        return 142;
    } else {
        return 100;
    }
}
/* Note to self: Any modifications to the following functions should be
 * reflected in the GoblinCaves project */
Vec2iList* bh_line(Vec2i start, Vec2i finish) {
    Vec2iList *results = NULL;
    int e,j,i;
    int dX = abs(start.x - finish.x);
    int dY = abs(start.y - finish.y);

    if(dX >= dY)
    {
        e = dY - dX; // [e]rror
        j = start.y;

        if(start.x < finish.x)
        {
            // Octants 1,2
            for(i = start.x; i <= finish.x; ++i)
            {
                bh_line_add(&results, make_vec(i,j));
                if((e >= 0) && (finish.y >= start.y))
                {
                    // 1
                    j += 1;
                    e -= dX;
                }
                else if((e >= 0) && (finish.y < start.y))
                {
                    // 2
                    j -= 1;
                    e -= dX;
                }
                e += dY;
            }
        }
        else if(start.x > finish.x)
        {
            // Octants 5,6
            for(i = start.x; i >= finish.x; --i)
            {
                bh_line_add(&results, make_vec(i, j));
                if((e >= 0) && (finish.y >= start.y))
                {
                    // 6
                    j += 1;
                    e -= dX;
                }
                else if((e >= 0) && (finish.y < start.y))
                {
                    // 5
                    j -= 1;
                    e -= dX;
                }
                e += dY;
            }
        }
    }
    else if (dX < dY)
    {
        e = dX - dY; // [e]rror
        i = start.x;
        if(start.y < finish.y)
        {
            // Octants 7,8
            for(j = start.y; j <= finish.y; ++j)
            {
                bh_line_add(&results, make_vec(i,j));
                if((e >= 0) && (finish.x >= start.x))
                {
                    // 8
                    i += 1;
                    e -= dY;
                }
                else if((e >= 0) && (finish.x < start.x))
                {
                    // 7
                    i -= 1;
                    e -= dY;
                }
                e += dX;
            }
        }
        else if(start.y > finish.y)
        {
            // Octants 3,4
            for(j = start.y; j >= finish.y; --j)
            {
                bh_line_add(&results, make_vec(i,j));
                if((e >= 0) && (finish.x >= start.x))
                {
                    // 3
                    i += 1;
                    e -= dY;
                }
                else if((e >= 0) && (finish.x < start.x))
                {
                    // 4
                    i -= 1;
                    e -= dY;
                }
                e += dX;
            }
        }
    }

    return results;
}

void bh_line_add(Vec2iList **head, Vec2i pos) {
    if(!Vec2i_list_contains(*head, pos)) {
        push_Vec2i_list(head, pos);
    }
}

Vec2iList* breadth_first_search(Vec2i start, bool monsterblock) {
    /* Breadth first search, returns ALL tiles that are not blocked.
     * Could be used for flood filling or other fun? Mostly here for comparison
     * in the future. */
    Vec2iList *reached = create_Vec2i_list(start);
    Vec2iList *frontier = create_Vec2i_list(start);
    Vec2iPQ *neighbors = NULL;
    Vec2iPQ *tmp = NULL;
    Vec2i cur = NULLVEC;

    while(frontier) {
        cur = pop_Vec2i_list(&frontier);
        neighbors = open_neighbors_at(cur, monsterblock);
        //count = count_Vec2i_list(neighbors);
        //for(i = 0; i < count; i++) {
        for(tmp = neighbors; tmp; tmp = tmp->next) {
            if(!Vec2i_list_contains(reached, tmp->item)) {
                push_Vec2i_list(&reached, tmp->item);
                push_Vec2i_list(&frontier, tmp->item);
            }
        }
        destroy_Vec2iPQ(&neighbors);
        neighbors = NULL;
    }

    destroy_Vec2i_list(&frontier);
    destroy_Vec2iPQ(&neighbors);
    return reached;
}

Vec2iHT* dijkstra_map(Vec2i start, bool monsterblock) {
    Vec2iPQ *frontier = create_Vec2iPQ(start, 0);
    Vec2iPQ *neighbors = NULL;
    Vec2iPQ *tmp = NULL;
    Vec2iHT *camefrom = create_Vec2iHT(5000); 
    Vec2iHT *costSoFar = create_Vec2iHT(5000); 
    Vec2i cost;

    int p = 0;
    int newcost = 0;
    Vec2i cur = NULLVEC;
    Vec2i next = NULLVEC;

    insert_Vec2iHT(camefrom, start, start);
    /* The cost is stored in the x of the value, y is 0 */
    /* If we pass this in, we should be able to get D maps for multiple things
     * by combining maps */
    insert_Vec2iHT(costSoFar, start, make_vec(0,0));
    while(frontier) {
        cur = pop_Vec2iPQ(&frontier);
        neighbors = open_neighbors_at(cur, monsterblock);
        for(tmp = neighbors; tmp; tmp = tmp->next) {
            next = tmp->item;
            cost = search_Vec2iHT(costSoFar, cur);
            newcost = cost.x + 1;//movement_cost_to(cur,next);//movement_cost_at(next);
            cost = search_Vec2iHT(costSoFar, next);
            if(vec_null(search_Vec2iHT(costSoFar, next)) || 
                    (newcost < cost.x)) {
                insert_Vec2iHT(costSoFar, next, make_vec(newcost, 0));
                p = newcost;
                push_Vec2iPQ(&frontier, next, p);
                insert_Vec2iHT(camefrom, next, cur);
            }
        }
        destroy_Vec2iPQ(&neighbors);
        neighbors = NULL;
    }
    //write_htable_csv(costSoFar,start,make_vec(0,0));
    write_dijkstra_map(costSoFar,start);
    destroy_Vec2iPQ(&frontier);
    destroy_Vec2iPQ(&neighbors);
    destroy_Vec2iHT(camefrom);
    return costSoFar;
}

Vec2i bfs_step(Vec2i start, Vec2i goal, bool monsterblock) {
    Vec2iList *path = bfs_path(start, goal, monsterblock);
    Vec2i result = start;
    if(path) {
        result = path->item;
    }
    destroy_Vec2i_list(&path);
    return result;
}

Vec2i gbfs_step(Vec2i start, Vec2i goal, bool monsterblock) {
    Vec2iList *path = gbfs_path(start, goal, monsterblock);
    Vec2i result = start;
    if(path) {
        result = path->item;
    }
    destroy_Vec2i_list(&path);
    return result;
}

Vec2i astar_step(Vec2i start, Vec2i goal, bool monsterblock) {
    Vec2iList *path = astar_path(start, goal, monsterblock);
    Vec2i result = start;
    if(path) {
        result = path->item;
    }
    destroy_Vec2i_list(&path);
    return result;
}

Vec2iList* construct_path(Vec2iHT *input, Vec2i start, Vec2i goal) {
    Vec2iList *path = NULL;
    Vec2i cur = goal;
    while(!eq_vec(cur, start)) {
        push_Vec2i_list(&path, cur);
        cur = search_Vec2iHT(input, cur);
    }
    return path;
}

Vec2iList* bfs_path(Vec2i start, Vec2i goal, bool monsterblock) {
    /* Best First Search with early exit, that returns a path from start
     * goal */
    Vec2iList *frontier = create_Vec2i_list(start);
    Vec2iPQ *neighbors = NULL;
    Vec2iPQ *tmp = NULL;
    Vec2iHT *camefrom = create_Vec2iHT(5000); 
    Vec2i cur = NULLVEC;
    Vec2i next = NULLVEC;
    Vec2iList *result = NULL;

    insert_Vec2iHT(camefrom, start, start);
    while(frontier) {
        cur = pop_Vec2i_list(&frontier);
        if(eq_vec(cur, goal)) {
            /* Early exit, found goal */
            break;
        }
        neighbors = open_neighbors_at(cur, monsterblock);
        for(tmp = neighbors; tmp; tmp = tmp->next) {
            next = tmp->item;
            if(vec_null(search_Vec2iHT(camefrom, next))) {
                push_Vec2i_list(&frontier, next);
                insert_Vec2iHT(camefrom, next, cur);
            }
        }
        destroy_Vec2iPQ(&neighbors);
        neighbors = NULL;
    }
    destroy_Vec2i_list(&frontier);
    destroy_Vec2iPQ(&neighbors);
    result = construct_path(camefrom, start, goal);
    //write_htable_csv(camefrom, start, goal);
    destroy_Vec2iHT(camefrom);
    return result;
}

Vec2iList* gbfs_path(Vec2i start, Vec2i goal, bool monsterblock) {
    /* Greedy BFS with early exit, returns path from start to goal.
     * Heuristic: Manhattan Distance */
    Vec2iPQ *frontier = create_Vec2iPQ(start, 0);
    Vec2iPQ *neighbors = NULL;
    Vec2iPQ *tmp = NULL;
    Vec2iHT *camefrom = create_Vec2iHT(5000); 
    Vec2iList *result = NULL;

    int p = 0;
    Vec2i cur = NULLVEC;
    Vec2i next = NULLVEC;

    insert_Vec2iHT(camefrom, start, start);
    while(frontier) {
        cur = pop_Vec2iPQ(&frontier);
        if(eq_vec(cur, goal)) {
            /* Early exit, found goal */
            break;
        }
        neighbors = open_neighbors_at(cur, monsterblock);
        for(tmp = neighbors; tmp; tmp = tmp->next) {
            next = tmp->item;
            if(vec_null(search_Vec2iHT(camefrom, next))) {
                p = man_dist(goal, next);
                push_Vec2iPQ(&frontier, next, p);
                insert_Vec2iHT(camefrom, next, cur);
            }
        }
        destroy_Vec2iPQ(&neighbors);
        neighbors = NULL;
    }
    destroy_Vec2iPQ(&frontier);
    destroy_Vec2iPQ(&neighbors);
    result = construct_path(camefrom, start, goal);
    //write_explored_map(camefrom, start, goal);
    //write_htable_csv(camefrom, start, goal);
    destroy_Vec2iHT(camefrom);
    return result;
}

Vec2iList* astar_path(Vec2i start, Vec2i goal, bool monsterblock) {
    Vec2iPQ *frontier = create_Vec2iPQ(start, 0);
    Vec2iPQ *neighbors = NULL;
    Vec2iPQ *tmp = NULL;
    Vec2iHT *camefrom = create_Vec2iHT(5000); 
    Vec2iHT *costSoFar = create_Vec2iHT(5000); 
    Vec2i cost;
    Vec2iList *result = NULL;

    int p = 0;
    int newcost = 0;
    Vec2i cur = NULLVEC;
    Vec2i next = NULLVEC;

    insert_Vec2iHT(camefrom, start, start);
    /* The cost is stored in the x of the value, y is 0 */
    insert_Vec2iHT(costSoFar, start, make_vec(0,0));
    while(frontier) {
        cur = pop_Vec2iPQ(&frontier);
        if(eq_vec(cur, goal)) {
            /* Early exit, found goal */
            break;
        }
        neighbors = open_neighbors_at(cur, monsterblock);
        for(tmp = neighbors; tmp; tmp = tmp->next) {
            next = tmp->item;
            cost = search_Vec2iHT(costSoFar, cur);
            newcost = cost.x + movement_cost_to(cur,next); //movement_cost_at(next);
            cost = search_Vec2iHT(costSoFar, next);
            if(vec_null(search_Vec2iHT(costSoFar, next)) || 
                    (newcost < cost.x)) {
                insert_Vec2iHT(costSoFar, next, make_vec(newcost, 0));
                p = newcost + man_dist(goal,next);
                push_Vec2iPQ(&frontier, next, p);
                insert_Vec2iHT(camefrom, next, cur);
            }
        }
        destroy_Vec2iPQ(&neighbors);
        neighbors = NULL;
    }
    result = construct_path(camefrom, start, goal);
    //write_dijkstra_map(costSoFar, start);
    //write_explored_map(costSoFar, start, goal);
    destroy_Vec2iPQ(&frontier);
    destroy_Vec2iPQ(&neighbors);
    destroy_Vec2iHT(costSoFar);
    destroy_Vec2iHT(camefrom);
    return result;
}

