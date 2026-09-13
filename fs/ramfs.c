#include "ramfs.h"
#include "../kernel/arch/x86_64/console.h"

static struct ramfs_file files[RAMFS_MAX_FILES];

void ramfs_init(void) {
    for (int i = 0; i < RAMFS_MAX_FILES; i++) files[i].used = 0;
    /* seed */
    const char* hello = "JagX RamFS root\n";
    ramfs_write("/readme.txt", (const uint8_t*)hello, 16);
    console_write("[RAMFS] Initialized\n");
}

static int find_by_name(const char* path) {
    for (int i = 0; i < RAMFS_MAX_FILES; i++) {
        if (!files[i].used) continue;
        int j = 0;
        while (path[j] && files[i].name[j] && path[j] == files[i].name[j]) j++;
        if (path[j] == 0 && files[i].name[j] == 0) return i;
    }
    return -1;
}

int ramfs_write(const char* path, const uint8_t* data, uint32_t len) {
    if (!path || !data) return -1;
    if (len > RAMFS_MAX_SIZE) len = RAMFS_MAX_SIZE;
    int idx = find_by_name(path);
    if (idx < 0) {
        for (int i = 0; i < RAMFS_MAX_FILES; i++) {
            if (!files[i].used) { idx = i; break; }
        }
        if (idx < 0) return -1;
        files[idx].used = 1;
        int j = 0;
        while (path[j] && j < RAMFS_MAX_NAME - 1) {
            files[idx].name[j] = path[j]; j++;
        }
        files[idx].name[j] = 0;
    }
    for (uint32_t i = 0; i < len; i++) files[idx].data[i] = data[i];
    files[idx].size = len;
    return 0;
}

int ramfs_read(const char* path, uint8_t* buf, uint32_t buf_len, uint32_t* out_len) {
    int idx = find_by_name(path);
    if (idx < 0) return -1;
    uint32_t n = files[idx].size < buf_len ? files[idx].size : buf_len;
    for (uint32_t i = 0; i < n; i++) buf[i] = files[idx].data[i];
    if (out_len) *out_len = n;
    return 0;
}

void ramfs_list(const char* dir) {
    (void)dir;
    for (int i = 0; i < RAMFS_MAX_FILES; i++) {
        if (!files[i].used) continue;
        console_write("  ");
        console_write(files[i].name);
        console_write(" (");
        console_write_dec(files[i].size);
        console_write(" bytes)\n");
    }
}

int ramfs_count(void) {
    int c = 0;
    for (int i = 0; i < RAMFS_MAX_FILES; i++) if (files[i].used) c++;
    return c;
}

const char* ramfs_name_at(int index) {
    int seen = 0;
    for (int i = 0; i < RAMFS_MAX_FILES; i++) {
        if (!files[i].used) continue;
        if (seen == index) return files[i].name;
        seen++;
    }
    return 0;
}
