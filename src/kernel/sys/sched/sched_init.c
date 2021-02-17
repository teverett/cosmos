/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/collection/linkedlist/linkedlist.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/sched/sched.h>

#include <sys/kprintf/kprintf.h>

linkedlist*** task_list;

void sched_init() {
    /*
     * Initialize task_list array.  Right now we only support a single
     * processor, so this is simpler than it will become in the future.
     */

    task_list = (linkedlist***)kmalloc(sizeof(linkedlist*));

    task_list[0] = (linkedlist**)kmalloc(sizeof(linkedlist));
    task_list[0][0] = linkedlist_new();

    task_list[0][0]->next = task_list[0][0];

    task_list[0][0]->data = (void*)kmalloc(sizeof(scheduler_task_t));

    SCHEDULER_TASK_LIST(0, 0)->pid = 0;
    SCHEDULER_TASK_LIST(0, 0)->state = SCHED_SLEEPING;
    SCHEDULER_TASK_LIST(0, 0)->times_skipped = 0;
    SCHEDULER_TASK_LIST(0, 0)->exit_code = 0;

    return;
}