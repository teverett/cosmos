//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/block_util.h>
#include <obj/logical/fs/cpm/cpm.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objecttype/objecttype_block.h>
#include <sys/objecttype/objecttype_filesystem.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>

// https://obsolescence.wixsite.com/obsolescence/cpm-internals

#define CPM_MAX_FILES 64
#define CPM_FILE_ENTRY_LEN 32
#define CPM_SECTOR_SIZE 128
#define CPM_EMPTY 0xe5

struct cpm_file_entry {
    uint8_t used;
    uint8_t filename[8];
    uint8_t extension[3];
    uint8_t ex;
    uint8_t unused;
    uint8_t record_count;
    uint8_t disk_map[16];
    uint8_t cr;
    //   uint8_t dma[3];
} __attribute__((packed));

struct cpm_dir {
    struct cpm_file_entry files[CPM_MAX_FILES];
} __attribute__((packed));

struct cpm_objectdata {
    struct object* partition_objice;
    uint16_t block_device_sector_size;
    uint8_t sectors_needed_for_dir;
    uint32_t byte_size_dir;
} __attribute__((packed));

void cpm_format(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct cpm_objectdata* object_data = (struct cpm_objectdata*)obj->object_data;

    /*
    * make the dir entry
    */
    struct cpm_dir dir;
    memset((uint8_t*)&dir, 0, sizeof(struct cpm_dir));
    for (uint8_t i = 0; i < CPM_MAX_FILES; i++) {
        dir.files[i].used = CPM_EMPTY;
    }

    /*
    * figure out how many sectors we need
    */
    uint8_t* buffer = kmalloc(object_data->byte_size_dir);
    memset(buffer, 0, object_data->byte_size_dir);
    memcpy(buffer, (uint8_t*)&dir, sizeof(struct cpm_dir));
    for (uint8_t i = 0; i < object_data->sectors_needed_for_dir; i++) {
        blockutil_write(object_data->partition_objice, &(buffer[i]), object_data->byte_size_dir, i, 0);
    }
    kfree(buffer);
}

void cpm_read_dir(struct object* obj, struct cpm_dir* dir) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct cpm_objectdata* object_data = (struct cpm_objectdata*)obj->object_data;

    uint8_t* buffer = kmalloc(object_data->byte_size_dir);
    memset(buffer, 0, object_data->byte_size_dir);
    for (uint8_t i = 0; i < object_data->sectors_needed_for_dir; i++) {
        blockutil_read(object_data->partition_objice, &(buffer[i]), object_data->byte_size_dir, i, 0);
    }

    // copy to dest
    memcpy((uint8_t*)dir, buffer, sizeof(struct cpm_dir));

    kfree(buffer);
}

/*
 * perform device instance specific init here
 */
uint8_t cpm_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct cpm_objectdata* object_data = (struct cpm_objectdata*)obj->object_data;
    kprintf("Init %s on %s (%s)\n", obj->description, object_data->partition_objice->name, obj->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t cpm_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct cpm_objectdata* object_data = (struct cpm_objectdata*)obj->object_data;
    kprintf("Uninit %s on %s (%s)\n", obj->description, object_data->partition_objice->name, obj->name);
    kfree(obj->api);
    kfree(obj->object_data);
    return 1;
}

struct object* cpm_attach(struct object* partition_objice) {
    ASSERT(sizeof(struct cpm_file_entry) == CPM_FILE_ENTRY_LEN);
    ASSERT_NOT_NULL(partition_objice);
    ASSERT(1 == blockutil_is_block_object(partition_objice));
    /*
     * register device
     */
    struct object* objectinstance = objectmgr_new_object();
    objectinstance->init = &cpm_init;
    objectinstance->uninit = &cpm_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = FILESYSTEM;
    objectmgr_set_object_description(objectinstance, "CP/M File System");
    /*
     * the device api
     */
    struct objecttype_filesystem* api = (struct objecttype_filesystem*)kmalloc(sizeof(struct objecttype_filesystem));
    memzero((uint8_t*)api, sizeof(struct objecttype_filesystem));
    // api->format = &cpm_format;
    objectinstance->api = api;
    /*
     * device data
     */
    struct cpm_objectdata* object_data = (struct cpm_objectdata*)kmalloc(sizeof(struct cpm_objectdata));
    object_data->partition_objice = partition_objice;
    object_data->block_device_sector_size = blockutil_get_sector_size(object_data->partition_objice);
    // blocks needed for dir
    uint8_t sectors_needed = sizeof(struct cpm_dir) / object_data->block_device_sector_size;
    if (0 != (sizeof(struct cpm_dir) % object_data->block_device_sector_size)) {
        sectors_needed = sectors_needed + 1;
    }
    object_data->sectors_needed_for_dir = sectors_needed;
    object_data->byte_size_dir = object_data->sectors_needed_for_dir * object_data->block_device_sector_size;
    objectinstance->object_data = object_data;

    /*
     * register
     */
    if (0 != objectmgr_attach_object(objectinstance)) {
        /*
        * increase ref count of underlying device
        */
        objectmgr_increment_object_refcount(partition_objice);
        /*
        * return device
        */
        return objectinstance;
    } else {
        kfree(object_data);
        kfree(api);
        kfree(objectinstance);
        return 0;
    }
}

void cpm_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct cpm_objectdata* object_data = (struct cpm_objectdata*)obj->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->partition_objice);
    /*
    * detach
    */
    objectmgr_detach_object(obj);
}
