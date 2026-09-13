#ifndef JAGX_DATABASE_H
#define JAGX_DATABASE_H

/* JagBase — simple Access-like table store */

#define DB_MAX_ROWS 32
#define DB_MAX_COLS 6
#define DB_FIELD 32

void db_init(void);
int  db_add_row(const char* f0, const char* f1, const char* f2, const char* f3);
int  db_row_count(void);
const char* db_get(int row, int col);
void db_draw(int ox, int oy);
void db_list_console(void);

#endif
