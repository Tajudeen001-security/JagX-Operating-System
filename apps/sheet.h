#ifndef JAGX_SHEET_H
#define JAGX_SHEET_H

/* JagSheet — Excel-like grid for government / office demos */

#define SHEET_ROWS 12
#define SHEET_COLS 6
#define SHEET_CELL 24

void sheet_init(void);
void sheet_set(int row, int col, const char* text);
const char* sheet_get(int row, int col);
void sheet_set_number(int row, int col, int value);
int  sheet_sum_column(int col);
void sheet_draw(int ox, int oy);

#endif
