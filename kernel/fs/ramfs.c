#include "ramfs.h"
#include "../arch/x86_64/console.h"
#include "../mm/heap.h"
#include <string.h>   /* we will provide a tiny string.h later if needed */

static struct ramfs_file files[RAMFS_MAX_FILES];

/* Minimal string helpers (no libc yet) */
static size_t jagx_strlen(const char* s) {
    size_t n = 0;
    while (s[n]) n++;
    return n;
}

static int jagx_strcmp(const char* a, const char* b) {
    while (*a && (*a == *b)) { a++; b++; }
    return *(const unsigned char*)a - *(const unsigned char*)b;
}

static void jagx_strncpy(char* dst, const char* src, size_t n) {
    size_t i;
    for (i = 0; i < n - 1 && src[i]; i++) dst[i] = src[i];
    dst[i] = 0;
}

static void jagx_memcpy(void* dst, const void* src, size_t n) {
    uint8_t* d = dst;
    const uint8_t* s = src;
    for (size_t i = 0; i < n; i++) d[i] = s[i];
}

void ramfs_init(void) {
    for (int i = 0; i < RAMFS_MAX_FILES; i++) {
        files[i].used = 0;
        files[i].size = 0;
        files[i].name[0] = 0;
    }

    /* Create a couple of demo files */
    const char* welcome = "Welcome to JagX RamFS!\nThis is an in-memory filesystem.\n";
    ramfs_create("welcome.txt", welcome, jagx_strlen(welcome));

    const char* version = "JagX OS 0.0.2 - Early kernel with memory & FS stubs\n";
    ramfs_create("version", version, jagx_strlen(version));

    console_write("[RAMFS] Initialized with demo files\n");
}

int ramfs_create(const char* name, const void* data, size_t size) {
    if (size > RAMFS_MAX_SIZE) return -1;

    for (int i = 0; i < RAMFS_MAX_FILES; i++) {
        if (!files[i].used) {
            jagx_strncpy(files[i].name, name, RAMFS_MAX_NAME);
            jagx_memcpy(files[i].data, data, size);
            files[i].size = size;
            files[i].used = 1;
            return 0;
        }
    }
    return -1;  /* Full */
}

int ramfs_read(const char* name, void* buf, size_t max_size) {
    for (int i = 0; i < RAMFS_MAX_FILES; i++) {
        if (files[i].used && jagx_strcmp(files[i].name, name) == 0) {
            size_t to_copy = files[i].size < max_size ? files[i].size : max_size;
            jagx_memcpy(buf, files[i].data, to_copy);
            return (int)to_copy;
        }
    }
    return -1;  /* Not found */
}

void ramfs_list(void) {
    console_write("RamFS contents:\n");
    for (int i = 0; i < RAMFS_MAX_FILES; i++) {
        if (files[i].used) {
            console_write("  ");
            console_write(files[i].name);
            console_write("  (");
            console_write_dec(files[i].size);
            console_write(" bytes)\n");
        }
    }
}
