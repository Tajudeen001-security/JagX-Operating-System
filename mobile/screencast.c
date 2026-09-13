#include "screencast.h"
#include "gallery.h"
#include "../kernel/arch/x86_64/console.h"

static int recording = 0;
static uint32_t frames = 0;
static int rec_seq = 1;

int screencast_is_recording(void) { return recording; }

int screencast_start(void) {
    if (recording) return -1;
    recording = 1;
    frames = 0;
    console_write("[REC] Screen recording started\n");
    return 0;
}

int screencast_stop(void) {
    if (!recording) return -1;
    recording = 0;
    char name[48];
    name[0]='r';name[1]='e';name[2]='c';name[3]='o';name[4]='r';name[5]='d';
    name[6]='-'; name[7] = (char)('0' + (rec_seq % 10));
    name[8]='.'; name[9]='j'; name[10]='a'; name[11]='g'; name[12]='v'; name[13]=0;
    rec_seq++;
    gallery_add(JAGX_MEDIA_RECORDING, name, 1024, 768, frames * 1024);
    console_write("[REC] Saved to Gallery\n");
    frames = 0;
    return 0;
}

void screencast_tick(void) {
    if (recording) frames++;
}
