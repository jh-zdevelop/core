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

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "core.h"

#define DEFAULT_OUT_BUF_CAPACITY 2048

/* old settings for terminal */
static struct termios old_termios;

/* output buffer for the terminal */
static char* out_buf = NULL;
static int out_buf_cap = 0;
static int out_buf_len = 0;

/* current known terminal dimensions */
static int rows = 0;
static int cols = 0;

void term_init(void)
{
    /* already initialized */
    if (out_buf)
        return;

    out_buf_cap = DEFAULT_OUT_BUF_CAPACITY;
    out_buf_len = 0;
    out_buf = mem_alloc(out_buf_cap);

    if (tcgetattr(1, &old_termios) == -1)
        die("tcgetattr() failed");

    /* need to set up raw mode */
    struct termios raw = old_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(1, TCSAFLUSH, &raw) == -1)
        die("tcsetattr() failed");

    /* grab current terminal size */
    struct winsize ws;
    if (ioctl(1, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
        die("ioctl() failed - get window size");

    rows = ws.ws_row;
    cols = ws.ws_col;
}

void term_cleanup(void)
{
    if (out_buf)
    {
        mem_free(out_buf);
        out_buf = NULL;

        /* ignore errors as this probably is done at exit */
        (void)tcsetattr(1, TCSAFLUSH, &old_termios);
    }
}

/* add bytes to the output buffer */
static void out(const char* text)
{
    while (*text)
    {
        if (out_buf_len == out_buf_cap)
        {
            out_buf_cap *= 2;
            char* new_buf = mem_alloc(out_buf_cap);
            memcpy(new_buf, out_buf, out_buf_len);
            mem_free(out_buf);
            out_buf = new_buf;
        }

        out_buf[out_buf_len++] = *text++;
    }
}

void term_flush(void)
{
    if (out_buf_len > 0)
    {
        /* todo : check for errors? */
        (void)write(1, out_buf, out_buf_len);
        out_buf_len = 0;
    }
}

int term_readkey(void)
{
    return 0;
}

void term_clear(void)
{
    out("\x1b[2J\x1b[H");
}

int term_get_rows(void)
{
    return rows;
}

int term_get_cols(void)
{
    return cols;
}

void term_locate(int row, int col)
{
    /* clip to window size */
    if (row < 1)
        row = 1;
    if (col < 1)
        col = 1;
    if (row > rows)
        row = rows;
    if (col > cols)
        col = cols;

    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", row, col);
    out(buf);
}

void term_color(int fg, int bg)
{
    static int fg_map[] = { 30, 34, 32, 36, 31, 35, 33, 37, 90, 94, 92, 96, 91, 95, 93, 97 };
    static int bg_map[] = { 40, 44, 42, 46, 41, 45, 43, 47, 100, 104, 102, 106, 101, 105, 103, 107 };

    if (fg < 0)
        fg = 0;
    if (fg > 15)
        fg = 15;
    if (bg < 0)
        bg = 0;
    if (bg > 15)
        bg = 15;

    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dm", fg_map[fg], bg_map[bg]);
    out(buf);
}

void term_hide_cursor(void)
{
    out("\1b[25l");
}

void term_show_cursor(void)
{
    out("\1b[25h");
}

void term_print(const char* text)
{
    out(text);
}

void term_printf(const char* format, ...)
{
    char buf[1024];     /* todo : too limited? */
    va_list ap;

    va_start(ap, format);
    vsnprintf(buf, sizeof(buf), format, ap);
    va_end(ap);

    out(buf);
}
