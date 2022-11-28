#ifndef PROCESS_H
#define PROCESS_H

#include "stdint.h"
#include "inode.h"
#include "paging.h"

#define PROCESS_MAX_NUM_FD 3

struct fd {
    inode_t *inode;
};
typedef struct fd fd_t;

struct ps {
    uint32_t id;

    pde_t *pdt;

    uint32_t pdt_paddr;
    uint32_t code_vaddr;
    uint32_t stack_vaddr;
    uint32_t heap_vaddr;

    uint32_t kernel_stack_vaddr;

    fd_t file_descriptors[PROCESS_MAX_NUM_FD];
};
typedef struct ps ps_t;

ps_t *process_create(char *path, uint32_t id);

#endif /* PROCESS_H */