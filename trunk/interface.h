#ifndef _INTERFACE_
#define _INTERFACE_

#include "instruction_sheet.h"
#include <stdbool.h>


void generate_configuration_sheet(instruction_sheet sheet,const char* filepath);

bool set_delay_values(instruction_sheet sheet, const char* filename);

#endif
