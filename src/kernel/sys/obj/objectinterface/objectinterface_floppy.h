//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all floppy objects will implement
 */
#ifndef _OBJECTTYPE_FLOPPY_H
#define _OBJECTTYPE_FLOPPY_H

#include <sys/obj/objectmgr/objectmgr.h>

#include <types.h>

typedef void (*floppy_read_sector_function)(struct object* obj, uint32_t lba, uint8_t* data, uint16_t size);
typedef void (*floppy_write_sector_function)(struct object* obj, uint32_t lba, uint8_t* data, uint16_t size);
typedef void (*floppy_reset_function)(struct object* obj);

struct objectinterface_floppy {
    floppy_read_sector_function read;
    floppy_write_sector_function write;
    floppy_reset_function reset;
};

#endif