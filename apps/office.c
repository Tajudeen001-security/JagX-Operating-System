#include "office.h"
#include "notepad.h"
#include "paint.h"
#include "sheet.h"
#include "database.h"
#include "../kernel/arch/x86_64/console.h"

void office_init(void) {
    notepad_init();
    paint_init();
    sheet_init();
    db_init();
    console_write("[OFFICE] Notepad + Paint + JagSheet + JagBase\n");
}

void office_draw_all(void) {
    notepad_draw(20, 40, 280, 160);
    paint_draw_ui(320, 40);
    sheet_draw(20, 220);
    db_draw(450, 220);
}

void office_on_click(int x, int y) {
    paint_on_click(x, y, 320, 40);
}
