//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <devicemgr/devicetypes.h>
#include <panic/panic.h>

struct array* types;

void devicetypes_init() {
    types = array_new(MAX_DEVICE_TYPES);
    for (uint16_t i=0; i<MAX_DEVICE_TYPES;i++) {
        array_set(types, i,0);
    }
}

struct list* devicetypes_get_devicelist(deviceType dt) {
    if ((dt>=0) && (dt<MAX_DEVICE_TYPES)){
       return (struct list*) array_get(types, dt);
    } else {
        panic("Invalid device type passed to devicetypes_get_devicelist");
    }

}

void devicetypes_set_devicelist(deviceType dt, struct list* lst) {
    if ((dt>=0) && (dt<MAX_DEVICE_TYPES)){
        array_set(types, dt, lst);
    } else {
        panic("Invalid device type passed to devicetypes_set_devicelist");
    }
}

