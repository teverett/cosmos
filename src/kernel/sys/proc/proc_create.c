/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objects/objects.h>
#include <sys/proc/proc.h>
#include <sys/string/mem.h>
#include <sys/sync/sync.h>
#include <sys/x86-64/mm/pagetables.h>

pttentry proc_obtain_cr3();

pid_t proc_create() {
    proc_info_t* proc_info;

    proc_info = (proc_info_t*)kmalloc(sizeof(proc_info_t));
    ASSERT_NOT_NULL(proc_info);
    memset((uint8_t*)proc_info, 0, sizeof(proc_info_t));

    spinlock_acquire(&proc_table_lock);
    proc_info->pid = get_next_pid();

    proc_table_add(proc_info);

    spinlock_release(&proc_table_lock);
    return proc_info->pid;
}

pttentry proc_obtain_cr3() {
    uint64_t cr3_page;
    pttentry proc_cr3;

    cr3_page = slab_allocate(1, PDT_INUSE);
    ASSERT_NOT_NULL(cr3_page);

    proc_cr3 = cr3_page * PAGE_SIZE;  // we don't need to set any flags
    memset(CONV_PHYS_ADDR(proc_cr3), 0, PAGE_SIZE);

    return proc_cr3;
}

void setup_user_process(pid_t pid, object_handle_t exe_obj) {

    proc_table_get(pid)->cr3 = proc_obtain_cr3();
    kprintf("cr3: 0x%llX\n", proc_table_get(pid)->cr3);

    return;
}