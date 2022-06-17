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

/* Main will be chonky, gonna have all the non-portable stuff here */

/*********
 * Globals
 *********/
Vec2i g_player;
Vec2i g_goal;
Vec2iList *g_path;
int g_state; 

typedef enum {
    S_NONE = 0,
    S_BFS,
    S_GBFS,
    S_ASTAR,
    S_BHL
} GameStates;

/***********************
 * Function Declarations
 ***********************/
bool handle_input(void);
Vec2i move_step(void);
void update_path(void);
void draw(void);
bool curses_setup(void);
void next_level(void);

/***********
 * Functions
 ***********/
bool handle_input(void) {
    int input = getch();
    bool result = true;
    Vec2i newPos = g_player;
    switch(input) {
        case 'q':
            result = false;
            break;
        case KEY_UP:
        case 'k':
            newPos.y--;
            break;
        case KEY_DOWN:
        case 'j':
            newPos.y++;
            break;
        case KEY_LEFT:
        case 'h':
            newPos.x--;
            break;
        case KEY_RIGHT:
        case 'l':
            newPos.x++;
            break;
        case 'y':
            newPos.x--;
            newPos.y--;
            break;
        case 'u':
            newPos.x++;
            newPos.y--;
            break;
        case 'b':
            newPos.x--;
            newPos.y++;
            break;
        case 'n':
            newPos.x++;
            newPos.y++;
            break;
        case '>':
            next_level();
            newPos = g_player;
            break;
        case '<':
            destroy_map();
            g_map = generate_map();
            g_player = build_redblob();
            place_tile_at(make_vec(14,2),T_DN);
            g_goal = get_stair_pos();
            destroy_Vec2i_list(&g_path);
            update_path();
            newPos = g_player;
            break;
        case '1':
            g_state = S_BFS;
            update_path();
            break;
        case '2':
            g_state = S_GBFS;
            update_path();
            break;
        case '3':
            g_state = S_ASTAR;
            update_path();
            break;
        case '4':
            g_state = S_BHL;
            update_path();
            break;
        case '0':
            g_state = S_NONE;
            break;
        case '5':
            destroy_Vec2iHT(dijkstra_map(g_player, false));
            break;
        case '.':
            newPos = move_step();
            break;
        default:
            break;
    }
    if(!is_blocked(newPos) && (!eq_vec(g_player, newPos))){
        g_player = newPos;
        update_path();
    }

    return result;
}

Vec2i move_step(void) {
    Vec2i newPos = g_player;
    switch (g_state) {
        case S_BFS:
            newPos = bfs_step(g_player, g_goal, true);
            break;
        case S_GBFS:
            newPos = gbfs_step(g_player, g_goal, true);
            break;
        case S_ASTAR:
            newPos = astar_step(g_player, g_goal, true);
            break;
        default:
            break;
    }
    return newPos;
}

void update_path(void) {
    destroy_Vec2i_list(&g_path);
    switch (g_state) { 
        case S_BFS:
            g_path = bfs_path(g_player,g_goal, true);
            break;
        case S_GBFS:
            g_path = gbfs_path(g_player,g_goal, true);
            break;
        case S_ASTAR:
            g_path = astar_path(g_player,g_goal, true);
            break;
        case S_BHL:
            g_path = bh_line(g_player,g_goal);
            break;
        default:
            g_path = NULL;
    }
}

void draw(void) {
    int x,y;
    Tile tile;

    int xoffset = COLS / 2; /* terminal center x */
    int yoffset = LINES / 2; /* terminal center y */

    xoffset -= MAP_WIDTH / 2;
    yoffset -= MAP_HEIGHT / 2;
    int hcolor = BLACK;
    switch (g_state) {
        case S_BFS:
            hcolor = RED;
            break;
        case S_GBFS:
            hcolor = BROWN;
            break;
        case S_ASTAR:
            hcolor = GREEN;
            break;
        case S_BHL:
            hcolor = CYAN;
            break;
        default:
            break;
    }
    
    erase();
    for(x = 0; x < MAP_WIDTH; x++) {
        for(y = 0; y < MAP_HEIGHT; y++) {
            tile = g_map[x][y];
            if(Vec2i_list_contains(g_path, make_vec(x,y))){
                setcolor(BRIGHT_WHITE,hcolor);
                mvaddch(y + yoffset, x + xoffset, tile.ch);
                unsetcolor(BRIGHT_WHITE,hcolor);
            } else {
                setcolor(tile.fg, tile.bg);
                mvaddch(y + yoffset, x + xoffset, tile.ch);
                unsetcolor(tile.fg, tile.bg);
            }
        }
    }
    setcolor(BRIGHT_WHITE, hcolor);
    mvaddch(yoffset + g_player.y, xoffset + g_player.x, '@');
    switch (g_state) {
        case S_BFS:
            mvprintw(yoffset,xoffset, "[1] BFS: %d steps to goal.", 
                    count_Vec2i_list(g_path));
            break;
        case S_GBFS:
            mvprintw(yoffset,xoffset, "[2] GBFS: %d steps to goal.", 
                    count_Vec2i_list(g_path));
            break;
        case S_ASTAR:
            mvprintw(yoffset,xoffset, "[3] A*: %d steps to goal.", 
                    count_Vec2i_list(g_path));
            break;
        case S_BHL:
            mvprintw(yoffset,xoffset, "[4] Bresenham's Line: %d distance to goal.",
                    count_Vec2i_list(g_path));
            break;
        default:
            mvprintw(yoffset,xoffset, "Press: [1]BFS [2]GBFS [3]A* [4]Bresenham's Line [5] Export Dijkstra");
            break;
    }
    unsetcolor(BRIGHT_WHITE, hcolor);
    refresh();
}

bool curses_setup(void) {
    bool success = false;

    initscr();
    noecho();
    //nodelay(stdscr, true);
    curs_set(0);
    cbreak();
    keypad(stdscr, true);

    if(has_colors()) {
        success = true;
        start_color();
        init_colorpairs();
    } else {
        /* System does not support color, print error message */
        mvprintw(LINES / 2, (COLS / 2) - 28, "Your system does not support color, unable to start game!");
        getch();
    }
    return success;
}

void next_level(void) {
    destroy_map();
    g_map = generate_map();
    g_player = build_dungeon();
    g_goal = get_stair_pos();
    destroy_Vec2i_list(&g_path);
    switch(g_state) {
        case S_BFS:
            g_path = bfs_path(g_player, g_goal, true);
            break;
        case S_GBFS:
            g_path = gbfs_path(g_player, g_goal, true);
            break;
        case S_ASTAR:
            g_path = astar_path(g_player, g_goal, true);
            break;
        default:
            break;
    }
}

/******
 * Main
 ******/

int main(int argc, char *argv[]) {
    bool running = curses_setup();
    init_genrand(time(NULL));
    srand(time(NULL));

    g_map = generate_map();
    g_state = S_ASTAR;
    next_level();
    while(running) {
        draw();
        running = handle_input();
    }
    destroy_map();
    destroy_Vec2i_list(&g_path);
    endwin();
    return 0;
}
