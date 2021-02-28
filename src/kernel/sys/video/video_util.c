//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_bga.h>
#include <sys/devicemgr/device.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>
#include <sys/video/video_util.h>

void video_get_resolution(struct device* dev, struct deviceapi_resolution* resolution) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT_NOT_NULL(resolution);
    ASSERT(dev->devicetype == BGA);
    struct deviceapi_bga* api = (struct deviceapi_bga*)dev->api;
    if (0 != api->get_resolution) {
        (*api->get_resolution)(dev, resolution);
    }
}

void video_set_resolution(struct device* dev, struct deviceapi_resolution* resolution) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT(dev->devicetype == BGA);
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT_NOT_NULL(resolution);
    ASSERT(dev->devicetype == BGA);
    struct deviceapi_bga* api = (struct deviceapi_bga*)dev->api;
    if (0 != api->set_resolution) {
        (*api->set_resolution)(dev, resolution);
    }
}

uint32_t video_util_get_buffersize(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT(dev->devicetype == BGA);
    struct deviceapi_bga* api = (struct deviceapi_bga*)dev->api;
    if (0 != api->get_buffersize) {
        return (*api->get_buffersize)(dev);
    }
    return 0;
}

void video_util_blt(struct device* dev, uint8_t* buffer, uint32_t buffer_size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->api);
    ASSERT(dev->devicetype == BGA);
    struct deviceapi_bga* api = (struct deviceapi_bga*)dev->api;
    if (0 != api->blt) {
        (*api->blt)(dev, buffer, buffer_size);
    }
}
