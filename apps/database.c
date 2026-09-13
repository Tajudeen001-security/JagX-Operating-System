#include "database.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

static char table[DB_MAX_ROWS][DB_MAX_COLS][DB_FIELD];
static int rows = 0;

static void setf(int r, int c, const char* s) {
    int i = 0;
    if (s) while (s[i] && i < DB_FIELD - 1) { table[r][c][i] = s[i]; i++; }
    table[r][c][i] = 0;
}

void db_init(void) {
    rows = 0;
    /* header */
    setf(0, 0, "ID"); setf(0, 1, "Name"); setf(0, 2, "Dept"); setf(0, 3, "State");
    rows = 1;
    db_add_row("001", "Adebayo", "Health", "Lagos");
    db_add_row("002", "Okeke", "Education", "Enugu");
    db_add_row("003", "Bello", "Works", "Kano");
    db_add_row("004", "Mensah", "Finance", "Accra");
    console_write("[DB] JagBase ready (Access-like)\n");
}

int db_add_row(const char* f0, const char* f1, const char* f2, const char* f3) {
    if (rows >= DB_MAX_ROWS) return -1;
    setf(rows, 0, f0); setf(rows, 1, f1); setf(rows, 2, f2); setf(rows, 3, f3);
    rows++;
    return rows - 1;
}

int db_row_count(void) { return rows; }

const char* db_get(int row, int col) {
    if (row < 0 || col < 0 || row >= rows || col >= DB_MAX_COLS) return "";
    return table[row][col];
}

void db_list_console(void) {
    for (int r = 0; r < rows; r++) {
        console_write(table[r][0]); console_write(" | ");
        console_write(table[r][1]); console_write(" | ");
        console_write(table[r][2]); console_write(" | ");
        console_write(table[r][3]); console_write("\n");
    }
}

void db_draw(int ox, int oy) {
    if (!fb_is_ready()) return;
    int cw = 80, rh = 22;
    fb_fill_rect((uint32_t)ox, (uint32_t)oy, 340, (uint32_t)(rows * rh + 30), 0xFF1A1A24);
    fb_fill_rect((uint32_t)ox, (uint32_t)oy, 340, 24, 0xFF9B59B6);
    for (int r = 0; r < rows && r < 10; r++) {
        for (int c = 0; c < 4; c++) {
            uint32_t col = (r == 0) ? 0xFF2A3A4A : 0xFFF0F0F0;
            fb_fill_rect((uint32_t)(ox + c * cw + 4), (uint32_t)(oy + 28 + r * rh),
                         (uint32_t)(cw - 4), (uint32_t)(rh - 2), col);
        }
    }
}
