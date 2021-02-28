//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/x86-64/bridge/intelpciisa/intelpciisa.h>
#include <obj/x86-64/pci/devicetree.h>
#include <obj/x86-64/pci/pci_device.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objectmgr/object.h>
#include <sys/objectmgr/objectmgr.h>
#include <types.h>

struct intelisapca_deviceddata {
    uint64_t base;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t pciisa_init(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    struct intelisapca_deviceddata* object_data = (struct intelisapca_deviceddata*)dev->object_data;
    object_data->base = pci_calcbar(dev->pci);
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n", dev->description, dev->pci->irq,
            dev->pci->vendor_id, dev->pci->device_id, object_data->base, dev->name);
    return 1;
}

void pciisa_search_cb(struct pci_device* dev) {
    ASSERT_NOT_NULL(dev);
    /*
     * register device
     */
    struct object* deviceinstance = objectmgr_new_object();
    deviceinstance->init = &pciisa_init;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = BRIDGE;
    objectmgr_set_object_description(deviceinstance, "Intel PIIX3 PCI-to-ISA Bridge (Triton II)");
    /*
     * device data
     */
    struct intelisapca_deviceddata* object_data =
        (struct intelisapca_deviceddata*)kmalloc(sizeof(struct intelisapca_deviceddata));
    deviceinstance->object_data = object_data;
    /*
     * register
     */
    objectmgr_register_object(deviceinstance);
}

/**
 * find all bridge devices and register them
 */
void pciisa_bridge_register() {
    pci_objectmgr_search_devicetype(PCI_CLASS_BRIDGE, PCI_BRIDGE_SUBCLASS_ISA, &pciisa_search_cb);
}
