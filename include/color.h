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
#ifndef COLOR_H
#define COLOR_H

/*************
 * Color Table
 *************
 * Color table using 0x0 to 0xF for the color range on each RGB value, with 0x5
 * and 0xA as the 1/3rd and 2/3rds intensities respectively. 
 * 0000	    (#000)	    Black           0
 * 0001	    (#00A)	    Blue            1
 * 0010	    (#0A0)	    Green           2
 * 0011	    (#0AA)	    Cyan            3
 * 0100	    (#A00)	    Red             4
 * 0101	    (#A0A)	    Magenta         5
 * 0110	    (#A50)	    Brown           6
 * 0111	    (#AAA)	    White           7
 * 1000	    (#555)	    Bright Black    8
 * 1001	    (#55F)	    Bright Blue     9
 * 1010	    (#5F5)	    Bright Green    10
 * 1011	    (#5FF)	    Bright Cyan     11
 * 1100	    (#F55)	    Bright Red      12
 * 1101	    (#F5F)	    Bright Magenta  13
 * 1110	    (#FF5)	    Bright Yellow   14
 * 1111	    (#FFF)	    Bright White    15
 ************/

typedef enum {
    BLACK           = 0,
    BLUE            = 1,
    GREEN           = 2,
    CYAN            = 3,
    RED             = 4,
    MAGENTA         = 5,
    BROWN           = 6,
    WHITE           = 7,
    BRIGHT_BLACK    = 8,
    BRIGHT_BLUE     = 9,
    BRIGHT_GREEN    = 10,
    BRIGHT_CYAN     = 11,
    BRIGHT_RED      = 12,
    BRIGHT_MAGENTA  = 13,
    BRIGHT_YELLOW   = 14,
    BRIGHT_WHITE    = 15 
} Color;

/*******************
 * color.c functions
 *******************/
void init_colorpairs(void);
short curs_color(int fg);
int colornum(int fg, int bg);
bool is_bold(int fg);
void setcolor(int fg, int bg);
void unsetcolor(int fg, int bg);

#endif
