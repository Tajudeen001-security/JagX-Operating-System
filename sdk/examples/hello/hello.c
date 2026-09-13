/* Smallest JagX app. Pack with tools/pack_jagx.py — output is hello.jagx */
#include "../../jagx.h"

int main(void) {
    jagx_notify("Hello JagX", "This is a .jagx app, not an APK.");
    return 0;
}
