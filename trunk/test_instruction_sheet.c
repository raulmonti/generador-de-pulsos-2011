#include <stdio.h>
#include "instruction.h"
#include "instruction_sheet.h"
#include "phase.h"

int main(int argc, char *argv[]){
    instruction_sheet inst_sheet = instruction_sheet_create();
	instruction inst1;
	instruction inst2;
	instruction inst3;
	phase ph1;
	phase ph2;
	phase ph3;

	getchar();

	inst1 = instruction_create(1, LOOP, 12);
	instruction_set_duration(inst1, 34);
	inst2 = instruction_create(2, ACQUIERE, 28);
	instruction_set_duration(inst2, 58);
	inst3 = instruction_create(3, DELAY, 23);
	instruction_set_duration(inst3, 63);

	ph1 = phase_create(1);
	phase_add_value(ph1, 21);
	phase_add_value(ph1, 34);
	phase_add_value(ph1, 15);
    phase_add_value(ph1, 123);
	ph2 = phase_create(2);
	phase_add_value(ph2, 44);
	phase_add_value(ph2, 61);
	phase_add_value(ph2, 71);
	ph3 = phase_create(3);
	phase_add_value(ph3, 29);
	phase_add_value(ph3, 99);

	instruction_sheet_add_instruction(inst_sheet, inst1);
	instruction_sheet_add_instruction(inst_sheet, inst2);
	instruction_sheet_add_instruction(inst_sheet, inst3);

    instruction_sheet_add_phase(inst_sheet, ph1);
    instruction_sheet_add_phase(inst_sheet, ph2);
    instruction_sheet_add_phase(inst_sheet, ph3);   

  //  instruction_sheet_print(inst_sheet);

	inst_sheet = instruction_sheet_destroy(inst_sheet);

	getchar();

    return 0;
}

