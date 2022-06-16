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

Vec2i g_player;
Vec2i g_goal;
Vec2iList *g_path;
int g_state; /* 0 none, 1 bfs, 2 gbfs, 3 a* */

typedef enum {
    S_NONE = 0,
    S_BFS,
    S_GBFS,
    S_ASTAR
} GameStates;

bool handle_input(void);
void move_step(void);
void update_path(void);
void draw(void);
bool curses_setup(void);
void next_level(void);

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
        case '0':
            g_state = S_NONE;
            break;
        case '.':
            move_step();
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

void move_step(void) {
    Vec2i newPos;
    switch (g_state) {
        case S_BFS:
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
    g_player = newPos;
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
            hcolor = CYAN;
            break;
        case S_ASTAR:
            hcolor = MAGENTA;
            break;
        default:
            break;
    }
    
    for(x = 0; x < MAP_WIDTH; x++) {
        for(y = 0; y < MAP_HEIGHT; y++) {
            tile = g_map[x][y];
            if(Vec2i_list_contains(g_path, make_vec(x,y))){
                setcolor(WHITE,hcolor);
                mvaddch(y + yoffset, x + xoffset, tile.ch);
                unsetcolor(WHITE,hcolor);
            } else {
                setcolor(tile.fg, tile.bg);
                mvaddch(y + yoffset, x + xoffset, tile.ch);
                unsetcolor(tile.fg, tile.bg);
            }
        }
    }
    setcolor(BRIGHT_WHITE, GREEN);
    mvaddch(yoffset + g_player.y, xoffset + g_player.x, '@');
    unsetcolor(BRIGHT_WHITE, GREEN);
}

bool curses_setup(void) {
    bool success = false;

    initscr();
    noecho();
    /*nodelay(stdscr, true);*/
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

int main(int argc, char *argv[]) {
    bool running = curses_setup();
    init_genrand(time(NULL));
    srand(time(NULL));

    g_map = generate_map();
    g_state = S_NONE;
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
