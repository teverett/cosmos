//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/fs/vfs/vfs.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_filesystem.h>
#include <sys/devicemgr/device.h>
#include <sys/fs/fs_facade.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>

struct filesystem_node* fsfacade_get_fs_rootnode(struct device* filesystem_device) {
    ASSERT_NOT_NULL(filesystem_device);
    ASSERT_NOT_NULL(filesystem_device->device_data);
    struct deviceapi_filesystem* fs_api = (struct deviceapi_filesystem*)filesystem_device->api;
    ASSERT_NOT_NULL(fs_api->root);
    return (*fs_api->root)(filesystem_device);
}

void fsfacade_traverse_internal(struct filesystem_node* fs_node, fsfacade_traverse_function f, uint32_t depth) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->api);

    ASSERT_NOT_NULL(f);

    // call the callback
    (*f)(fs_node, depth);

    struct filesystem_directory dir;
    memzero((uint8_t*)&dir, sizeof(struct filesystem_directory));
    fsfacade_list_directory(fs_node, &dir);

    if (dir.count > 0) {
        for (uint32_t i = 0; i < dir.count; i++) {
            struct filesystem_node* child = fsfacade_find_node_by_id(fs_node, dir.ids[i]);
            ASSERT_NOT_NULL(child);
            fsfacade_traverse_internal(child, f, depth + 1);
        }
    }
}

/*
 * traverse
 */
void fsfacade_traverse(struct filesystem_node* fs_node, fsfacade_traverse_function f) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->api);

    ASSERT_NOT_NULL(f);
    fsfacade_traverse_internal(fs_node, f, 0);
}

void vfs_dump_traverse_function(struct filesystem_node* fs_node, uint32_t depth) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->api);

    for (int32_t i = 0; i < depth; i++) {
        kprintf(" ");
    }
    kprintf("%s %#llX\n", fs_node->name, fs_node->id);
}

void fsfacade_dump(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    fsfacade_traverse(fs_node, &vfs_dump_traverse_function);
}

void fsfacade_list_directory(struct filesystem_node* fs_node, struct filesystem_directory* dir) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->api);
    ASSERT_NOT_NULL(dir);
    struct deviceapi_filesystem* fs_api = (struct deviceapi_filesystem*)fs_node->filesystem_device->api;
    if (0 != fs_api->list) {
        (*fs_api->list)(fs_node, dir);
    }
}

struct filesystem_node* fsfacade_find_node_by_id(struct filesystem_node* fs_node, uint32_t id) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->api);
    struct deviceapi_filesystem* fs_api = (struct deviceapi_filesystem*)fs_node->filesystem_device->api;

    //  kprintf("fsfacade_find_node_by_id id %llu of parent %s\n", id, fs_node->name);
    if (0 != fs_api->find_id) {
        return (*fs_api->find_id)(fs_node, id);
    }
    return 0;
}

uint64_t fsfacade_size(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->api);
    struct deviceapi_filesystem* fs_api = (struct deviceapi_filesystem*)fs_node->filesystem_device->api;
    if (0 != fs_api->size) {
        return (*fs_api->size)(fs_node);
    }
    return 0;
}

struct filesystem_node* fsfacade_find_node_by_name(struct filesystem_node* fs_node, char* name) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->api);
    ASSERT_NOT_NULL(name);

    struct filesystem_directory dir;
    fsfacade_list_directory(fs_node, &dir);

    if (dir.count > 0) {
        for (uint32_t i = 0; i < dir.count; i++) {
            uint64_t id = dir.ids[i];
            struct filesystem_node* node = fsfacade_find_node_by_id(fs_node, id);
            ASSERT_NOT_NULL(node);
            if (strcmp(node->name, name) == 0) {
                return node;
            }
        }
    }
    return 0;
}

uint32_t fsfacade_read(struct filesystem_node* fs_node, uint8_t* data, uint32_t data_size) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->api);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);
    struct deviceapi_filesystem* fs_api = (struct deviceapi_filesystem*)fs_node->filesystem_device->api;
    if (0 != fs_api->read) {
        return (*fs_api->read)(fs_node, data, data_size);
    }
    return 0;
}

uint32_t fsfacade_write(struct filesystem_node* fs_node, const uint8_t* data, uint32_t data_size) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->api);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);

    struct deviceapi_filesystem* fs_api = (struct deviceapi_filesystem*)fs_node->filesystem_device->api;
    if (0 != fs_api->write) {
        return (*fs_api->write)(fs_node, data, data_size);
    }
    return 0;
}
