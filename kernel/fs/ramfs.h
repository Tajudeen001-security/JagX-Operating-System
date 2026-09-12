#ifndef JAGX_RAMFS_H
#define JAGX_RAMFS_H

#include <stdint.h>
#include <stddef.h>

#define RAMFS_MAX_FILES 32
#define RAMFS_MAX_NAME  32
#define RAMFS_MAX_SIZE  4096

struct ramfs_file {
    char     name[RAMFS_MAX_NAME];
    uint8_t  data[RAMFS_MAX_SIZE];
    size_t   size;
    int      used;
};

void ramfs_init(void);
int  ramfs_create(const char* name, const void* data, size_t size);
int  ramfs_read(const char* name, void* buf, size_t max_size);
void ramfs_list(void);

#endif
