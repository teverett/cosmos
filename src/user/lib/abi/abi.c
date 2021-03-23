//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <abi/abi.h>
#include <abi/syscall.h>

uint64_t syscall_process_exit() {
    return syscall(SYSCALL_PROCESS_EXIT, 0, 0, 0);
}

uint64_t syscall_process_sleep(uint64_t time) {
    return syscall(SYSCALL_PROCESS_SLEEP, time, 0, 0);
}

uint64_t syscall_console_write(uint8_t* str) {
    return syscall(SYSCALL_CONSOLE_WRITE, (uint64_t)str, 0, 0);
}

uint64_t syscall_memory_malloc(uint64_t size) {
    return syscall(SYSCALL_MEMORY_MALLOC, size, 0, 0);
}

uint64_t syscall_memory_free(void* mem) {
    return syscall(SYSCALL_MEMORY_FREE, (uint64_t)mem, 0, 0);
}

uint64_t syscall_memory_realloc(void* mem, uint64_t size) {
    return syscall(SYSCALL_MEMORY_REALLOC, (uint64_t)mem, size, 0);
}

uint64_t syscall_serial_writechar(uint64_t c) {
    return syscall(SYSCALL_SERIAL_WRITECHAR, c, 0, 0);
}
uint64_t syscall_serial_readchar() {
    return syscall(SYSCALL_SERIAL_READCHAR, 0, 0, 0);
}

uint64_t syscall_objectmgr_get_device_by_name(uint8_t* name) {
    return syscall(SYSCALL_OBJMGR_GET_DEVICE_BY_NAME, (uint64_t)name, 0, 0);
}

uint64_t syscall_objectmgr_get_device_by_handle(uint64_t handle) {
    return syscall(SYSCALL_OBJMGR_GET_DEVICE_BY_HANDLE, handle, 0, 0);
}
