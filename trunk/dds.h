#ifndef _DDS_H
#define _DDS_H

#include <stdbool.h>
#include "lpt_linux.h"

#define RAM_REG_COM1   0x70
#define RAM_REG_WRITE  0x74

#define DDS_REG_COM1   0x71
#define DDS_REG_COM2   0x72
#define DDS_REG_WRITE  0x78
#define DDS_REG_DIR    0x75
#define DDS_REG_TRANSF 0x76
#define DDS_REG_COD    0x77


bool dds_set_address(unsigned char address);
bool dds_write(unsigned char data);
bool dds_send_word(unsigned int word);

void load_ram_phase(unsigned int mem_address, unsigned int phase_value);

#endif

