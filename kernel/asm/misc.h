
#ifndef _MISC_H
#define _MISC_H

#include <mm/mm.h>

void asm_cli();
void asm_hlt();
void asm_sti();
pttentry asm_cr3_read();

#endif