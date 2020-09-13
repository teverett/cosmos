/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _PANIC_C
#define _PANIC_C

#include <asm/asm.h>
#include <console/drivers/drivers.h>

void panic(const char *s){
	console_write("!!!PANIC!!!");
	console_write(s);
	
	asm_cli();
	asm_hlt();
}

#endif