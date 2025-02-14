//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_cpu.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <tests/obj/test_cpu.h>
#include <types.h>

void show_cpu_data() {
    // get the CPU
    struct object* cpu = objectmgr_find_object_by_name("cpu0");
    if (0 != cpu) {
        struct objectinterface_cpu* cpu_api = (struct objectinterface_cpu*)cpu->api;
        /*
         * show all CPU features
         */
        struct cpu_id id;
        (*cpu_api->features)(&id);
        kprintf("CPU Features %#llX\n", id.edx);

        /*
         * check if APIC
         */
        bool apic = (*cpu_api->apic)();
        if (apic) {
            kprintf("APIC present\n");
        } else {
            kprintf("APIC not present\n");
        }

        /*
         * show CPU manufacturer
         */
        uint8_t cpu_manufacturer_string[13];
        (*cpu_api->manufacturer)((uint8_t*)&cpu_manufacturer_string);
        kprintf("CPU Manufacturer: %s\n", cpu_manufacturer_string);
    } else {
        kprintf("Unable to find cpu0\n");
    }
}
