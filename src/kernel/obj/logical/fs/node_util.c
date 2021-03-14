//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/node_util.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>

struct object;

struct filesystem_node* filesystem_node_new(enum filesystem_node_type type, struct object* obj, const uint8_t* name,
                                            uint64_t size, uint64_t id, void* node_data, uint64_t parent) {

    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(name);
    struct filesystem_node* ret = (struct filesystem_node*)kmalloc(sizeof(struct filesystem_node));
    memzero((uint8_t*)ret, sizeof(struct filesystem_node));
    ret->type = type;
    ret->filesystem_obj = obj;
    strncpy(ret->name, name, FILESYSTEM_MAX_NAME);
    ret->id = id;
    ret->parent = parent;
    ret->node_data = node_data;
    ret->size = size;
    return ret;
}
