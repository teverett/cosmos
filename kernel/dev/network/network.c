// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/network/network.h>

void network_register_devices() {
    rtl8139_register_devices();
    ne2000pci_register_devices();
    ne2000isa_register_devices();
    e1000_register_devices();
}
