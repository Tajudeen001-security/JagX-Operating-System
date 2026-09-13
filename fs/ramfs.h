#ifndef JAGX_RAMFS_H
#define JAGX_RAMFS_H

#include <stdint.h>

#define RAMFS_MAX_FILES 32
#define RAMFS_MAX_NAME  48
#define RAMFS_MAX_SIZE  4096

struct ramfs_file {
    int used;
    char name[RAMFS_MAX_NAME];
    uint32_t size;
    uint8_t data[RAMFS_MAX_SIZE];
};

void ramfs_init(void);
int  ramfs_write(const char* path, const uint8_t* data, uint32_t len);
int  ramfs_read(const char* path, uint8_t* buf, uint32_t buf_len, uint32_t* out_len);
void ramfs_list(const char* dir);
int  ramfs_count(void);
const char* ramfs_name_at(int index);

#endif
