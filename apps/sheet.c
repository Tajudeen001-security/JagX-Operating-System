#include "sheet.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

static char cells[SHEET_ROWS][SHEET_COLS][SHEET_CELL];
static int nums[SHEET_ROWS][SHEET_COLS];
static int has_num[SHEET_ROWS][SHEET_COLS];

void sheet_init(void) {
    for (int r = 0; r < SHEET_ROWS; r++)
        for (int c = 0; c < SHEET_COLS; c++) {
            cells[r][c][0] = 0;
            nums[r][c] = 0;
            has_num[r][c] = 0;
        }
    /* Sample government budget demo */
    sheet_set(0, 0, "Item");
    sheet_set(0, 1, "Budget");
    sheet_set(0, 2, "Spent");
    sheet_set(1, 0, "Health");
    sheet_set_number(1, 1, 500);
    sheet_set_number(1, 2, 320);
    sheet_set(2, 0, "Education");
    sheet_set_number(2, 1, 800);
    sheet_set_number(2, 2, 610);
    sheet_set(3, 0, "Roads");
    sheet_set_number(3, 1, 450);
    sheet_set_number(3, 2, 200);
    console_write("[SHEET] JagSheet ready (Excel-like)\n");
}

void sheet_set(int row, int col, const char* text) {
    if (row < 0 || col < 0 || row >= SHEET_ROWS || col >= SHEET_COLS) return;
    int i = 0;
    if (text) while (text[i] && i < SHEET_CELL - 1) { cells[row][col][i] = text[i]; i++; }
    cells[row][col][i] = 0;
    has_num[row][col] = 0;
}

const char* sheet_get(int row, int col) {
    if (row < 0 || col < 0 || row >= SHEET_ROWS || col >= SHEET_COLS) return "";
    return cells[row][col];
}

void sheet_set_number(int row, int col, int value) {
    if (row < 0 || col < 0 || row >= SHEET_ROWS || col >= SHEET_COLS) return;
    nums[row][col] = value;
    has_num[row][col] = 1;
    /* simple itoa into cell */
    char tmp[SHEET_CELL];
    int v = value, neg = 0, p = 0;
    if (v < 0) { neg = 1; v = -v; }
    if (v == 0) tmp[p++] = '0';
    char rev[16]; int r = 0;
    while (v > 0 && r < 15) { rev[r++] = (char)('0' + (v % 10)); v /= 10; }
    if (neg) tmp[p++] = '-';
    while (r > 0) tmp[p++] = rev[--r];
    tmp[p] = 0;
    sheet_set(row, col, tmp);
    has_num[row][col] = 1;
    nums[row][col] = value;
}

int sheet_sum_column(int col) {
    int s = 0;
    for (int r = 1; r < SHEET_ROWS; r++)
        if (has_num[r][col]) s += nums[r][col];
    return s;
}

void sheet_draw(int ox, int oy) {
    if (!fb_is_ready()) return;
    int cw = 70, rh = 22;
    fb_fill_rect((uint32_t)ox, (uint32_t)oy, (uint32_t)(SHEET_COLS * cw + 4),
                 (uint32_t)(SHEET_ROWS * rh + 30), 0xFF1A1A24);
    fb_fill_rect((uint32_t)ox, (uint32_t)oy, (uint32_t)(SHEET_COLS * cw + 4), 24, 0xFF2ECC71);
    for (int r = 0; r < SHEET_ROWS; r++) {
        for (int c = 0; c < SHEET_COLS; c++) {
            uint32_t col = (r == 0) ? 0xFF2A3A4A : 0xFFF8F8F8;
            fb_fill_rect((uint32_t)(ox + c * cw + 2), (uint32_t)(oy + 26 + r * rh),
                         (uint32_t)(cw - 2), (uint32_t)(rh - 2), col);
        }
    }
}
