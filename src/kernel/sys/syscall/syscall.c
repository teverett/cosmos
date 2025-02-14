//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/kprintf/kprintf.h>
#include <sys/syscall/syscall.h>
uint64_t invalid_syscall(uint64_t syscall_id, struct syscall_args* args) {
    kprintf("Invalid syscall %llu\n", syscall_id);
    return 0;
}
