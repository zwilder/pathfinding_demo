/*
* Goblin Caves
* Copyright (C) Zach Wilder 2022
* 
* This file is a part of Goblin Caves
*
* Goblin Caves is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* Goblin Caves is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with Goblin Caves.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <pathfinding.h>

/* 
 * Credit for this goes to Jim Hall, "About ncurses Colors"
 * https://www.linuxjournal.com/content/about-ncurses-colors-0
 *
 * I **finally** understand how curses colors can work for me, and a whole lot
 * more about bit math. The code below is his, with a few minor style changes
 * and minor edits. The comments are mostly mine, and will be left here so I can
 * remind myself in the future how this works.
 */

void init_colorpairs(void) {
    /*
     * Function to create all possible color pairs of foreground/background with
     * the 8 curses colors
     */
    int fg, bg;
    int colorpair;
    for(bg = 0; bg <= 7; bg++) {
        for(fg = 0; fg <= 7; fg++) {
            colorpair = colornum(fg,bg);
            init_pair(colorpair,curs_color(fg),curs_color(bg));
        }
    }
}

short curs_color(int fg) {
    /*
     * Converts color number to curses color
     */
    switch(7 & fg) {                    /* RGB */
        case 0: return (COLOR_BLACK);   /* 000 */
        case 1: return (COLOR_BLUE);    /* 001 */
        case 2: return (COLOR_GREEN);   /* 010 */
        case 3: return (COLOR_CYAN);    /* 011 */
        case 4: return (COLOR_RED);     /* 100 */
        case 5: return (COLOR_MAGENTA); /* 101 */
        case 6: return (COLOR_YELLOW);  /* 110 */
        case 7: return (COLOR_WHITE);   /* 111 */
    }
    return COLOR_BLACK; /* Shouldn't reach here, but black if do */
}

int colornum(int fg, int bg) {
    /* 
     * "To create a predictable color pair number for each foreground and
     * background color, I also need a function colornum() to set an integer bit
     * pattern based on the classic color byte"
     *
     * Bbbb ffff
     * B Blink
     * bbb - three bits for the background color
     * ffff - four for the foreground color (BLack-Bright White, above)
     *
     * This should always return an 8 bit, positive integer - uint8_t (char).
     */

    int B, bbb, ffff;

    B = 1 << 7;
    bbb = (7 & bg) << 4;
    ffff = (7 & fg);
    
    /* Binary OR copies bits if it exists in either operand */
    return (B | bbb | ffff);
}

/* Neat trick here, since the binary AND will only return a non-zero number
 * if the result is 1000 to 1111.
 * EX Bright Black & Bright Cyan = (1000 & 1011) = 1000 = 15
 *    Bright Black & Cyan = (1000 & 0011) = 0000 = 0
 * SO, really, this can be used as a true/false.
 */
bool is_bold(int fg) {
    return (((1 << 3) & fg) > 0);
}

void setcolor(int fg, int bg) {
    /* Set the color pair (colornum) and bold/bright (A_BOLD) */
    attron(COLOR_PAIR(colornum(fg,bg)));
    if(is_bold(fg)) {
        attron(A_BOLD);
    }
}

void unsetcolor(int fg, int bg) {
    /* Unset the color pair (colornum) and bold/bright (A_BOLD) */
    attroff(COLOR_PAIR(colornum(fg,bg)));
    if(is_bold(fg)) {
        attroff(A_BOLD);
    }
}
