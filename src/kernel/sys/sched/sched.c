/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/objects/objects.h>
#include <sys/panic/panic.h>
#include <sys/sched/sched.h>

void sched_set_state(object_handle_t obj, scheduler_state_t state) {
    linkedlist* task;

    if (object_type_(obj) != OBJECT_TASK) {
        PANIC("Invalid object type!");
    }

    task = OBJECT_DATA(obj, object_task_t)->sched_task;
    TASK_DATA(task)->state = state;

    return;
}

void sched_switch(linkedlist* task) {
    object_handle_t task_obj, proc_obj, body_obj;

    task_obj = TASK_DATA(task)->obj;

    switch (object_type_(task_obj)) {
        case OBJECT_TASK:
            break;
        default:
            PANIC("Invalid object type!");
    }

    proc_obj = OBJECT_DATA(task_obj, object_task_t)->process;

    body_obj = OBJECT_DATA(proc_obj, object_process_t)->body;

    switch (object_type_(body_obj)) {
        case OBJECT_KERNEL_WORK:
            current_task[CUR_CPU][CUR_CORE] = task;
            OBJECT_DATA(body_obj, object_kernel_work_t)->work_func(OBJECT_DATA(body_obj, object_kernel_work_t)->arg);
            break;
        case OBJECT_EXECUTABLE:
            current_task[CUR_CPU][CUR_CORE] = task;
            switch_to_task(task);
            break;
        default:
            PANIC("Invalid object type!")
    }

    return;
}