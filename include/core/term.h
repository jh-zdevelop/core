/*
 * MIT License
 *
 * Copyright (c) 2018 Jason Hoyt
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

enum
{
    C_BLACK,
    C_BLUE,
    C_GREEN,
    C_CYAN,
    C_RED,
    C_PURPLE,
    C_YELLOW,
    C_GRAY,
    C_DARK_GRAY,
    C_LIGHT_BLUE,
    C_LIGHT_GREEN,
    C_LIGHT_CYAN,
    C_LIGHT_RED,
    C_LIGHT_PURPLE,
    C_LIGHT_YELLOW,
    C_WHITE
};

/* initialize the terminal sub-system */
void term_init(void);

/* cleanup the terminal sub-system */
void term_cleanup(void);

/* flush pending writes to the terminal */
void term_flush(void);

/* read the next key event from the terminal (or 0 for none) */
int term_readkey(void);

/* clear the terminal screen, move cursor to upper-left */
void term_clear(void);

/* retrieve number of rows for the terminal */
int term_get_rows(void);

/* return number of columns for the terminal */
int term_get_cols(void);

/* move the terminal cursor position for future writes */
void term_locate(int row, int col);

/* set the terminal color for future writes */
void term_color(int fg, int bg);

/* hide the terminal cursor */
void term_hide_cursor(void);

/* show the terminal cursor */
void term_show_cursor(void);

/* write text to the terminal */
void term_print(const char* text);

/* write formatted text to the terminal */
void term_printf(const char* format, ...);
