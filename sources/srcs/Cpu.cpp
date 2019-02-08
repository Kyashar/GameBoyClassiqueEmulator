/*
** EPITECH PROJECT, 2019
** Epitech scolarship project (4 years remaining)
** File description:
**      Made on 2019/01 by ebernard
*/

#include <iostream>
#include "Cpu.hpp"

emulator::Cpu::Cpu(std::vector<uint8_t> &instructions) :
	_gpu(_memory), _read(true)
{
	_register.pc = 0;
	_register.sp = 0;
	_memory.loadRom(instructions);
	_romSize = instructions.size();

	_gpu.initWindow();
}

emulator::Cpu::~Cpu()
{

}

bool emulator::Cpu::gotSomethingToRead() const
{
	return _read;
}

void emulator::Cpu::readInstruction()
{
	uint8_t cData = 0;
  	uint16_t sData = 0;

	if (managedInstruction[_memory[_register.pc]]._length == 2) {
		cData = *((unsigned char *) (&_memory[_register.pc]));
		sData = cData;

	} else if (managedInstruction[_memory[_register.pc]]._length == 3)
		sData = _memory.getShort(_register.pc);

	(*this.*managedInstruction[_memory[_register.pc]]._instruction)(sData);

	_register.t = managedInstruction[_memory[_register.pc]]._timer;
	_register.m = _register.t * 4;

	_register.pc += managedInstruction[_memory[_register.pc]]._length;
	if (_register.pc > _romSize)
		exit(1);
//	_gpu.put(managedInstruction[_memory[_register.pc]]._timer);
}

std::ostream &operator<<(std::ostream &os, const emulator::Cpu::instructionInfos &infos)
{
	os << infos._name;
	return os;
}

std::vector<emulator::Cpu::instructionInfos> emulator::Cpu::managedInstruction = {
	{"NOP", 1, &Cpu::Nop, 1}, /* 0x00 */
	{"LD BC, d16", 3, &Cpu::Ld_BC, 3},
	{"LD (BC), A", 1, &Cpu::Ld_BC_A, 2},
	{"INC BC", 1, &Cpu::Inc_BC, 2},
	{"INC B", 1, &Cpu::Inc_B, 1},
	{"DEC B", 1, &Cpu::Dec_B, 1},
	{"LD B, d8", 2, &Cpu::Ld_B, 2},
	{"RLCA", 1, &Cpu::Rlca, 1},
	{"LD (a16), SP", 3, &Cpu::Ld_SP, 5},
	{"ADD HL, BC", 1, &Cpu::Add_HL_BC, 2},
	{"LD A, (BC)", 1, &Cpu::Ld_A_BC, 2},
	{"DEC BC", 1, &Cpu::Dec_BC, 2},
	{"INC C", 1, &Cpu::Inc_C, 1},
	{"DEC C", 1, &Cpu::Dec_C, 1},
	{"LD C, d8", 2, &Cpu::Ld_C, 2},
	{"RRCA", 1, &Cpu::Rrca, 1},

	{"STOP 0", 2, &Cpu::Stop, 1}, /* 0x10 */
	{"LD DE, d16", 3, &Cpu::Ld_DE, 3},
	{"LD (DE), A", 1, &Cpu::Ld_DE_A, 2},
	{"INC DE", 1, &Cpu::Inc_DE, 2},
	{"INC D", 1, &Cpu::Inc_D, 1},
	{"DEC D", 1, &Cpu::Dec_D, 1},
	{"LD D, d8", 2, &Cpu::Ld_D, 2},
	{"RLA", 1, &Cpu::Rla, 1},
	{"JR r8", 2, &Cpu::Jr, 3},
	{"ADD HL, DE", 1, &Cpu::Add_Hl_De, 2},
	{"LD A, (DE)", 1, &Cpu::Ld_A_De, 2},
	{"DEC DE", 1, &Cpu::Dec_De, 2},
	{"INC E", 1, &Cpu::Inc_E, 1},
	{"DEC E", 1, &Cpu::Dec_E, 1},
	{"LD E, d8", 2, &Cpu::Ld_E, 2},
	{"RRA", 1, &Cpu::Rra, 1},

	{"JR NZ, r8", 2, &Cpu::Jr_Nz, 3}, // 2 /* 0x20 */
	{"LD HL, d16", 3, &Cpu::Ld_Hl, 3},
	{"LD (HL+), A", 1, &Cpu::Ld_Hlpp_A, 2},
	{"INC HL", 1, &Cpu::Inc_Hl, 2},
	{"INC H", 1, &Cpu::Inc_H, 1},
	{"DEC H", 1, &Cpu::Dec_H, 1},
	{"LD H, d8", 2, &Cpu::Ld_H, 2},
	{"DAA", 1, &Cpu::Daa, 1},
	{"JR Z, r8", 2, &Cpu::Jr_Z, 3}, // 2
	{"ADD HL, HL", 1, &Cpu::Add_Hl_Hl, 2},
	{"LD A, (HL+)", 1, &Cpu::Ld_A_Hlpp, 2},
	{"DEC HL", 1, &Cpu::Dec_Hl, 2},
	{"INC L", 1, &Cpu::Inc_L, 1},
	{"DEC L", 1, &Cpu::Dec_L, 1},
	{"LD L, d8", 2, &Cpu::Ld_L, 2},
	{"CPL", 1, &Cpu::Cpl, 1},

	{"JR NC, r8", 2, &Cpu::Jr_Nc, 3},//2 /* 0x30 */
	{"LD SP, d16", 3, &Cpu::Ld_Sp, 3},
	{"LD (HL-), A", 1, &Cpu::Ld_Hln_A, 2},
	{"INC SP", 1, &Cpu::Inc_Sp, 2},
	{"INC (HL)", 1, &Cpu::Inc_Hlp, 3},
	{"DEC (HL)", 1, &Cpu::Dec_Hlp, 3},
	{"LD (HL), d8", 2, &Cpu::Ld_Hlp, 3},
	{"SCF", 1, &Cpu::Scf, 1},
	{"JR C, r8", 2, &Cpu::Jr_C, 3}, // 2
	{"ADD HL, SP", 1, &Cpu::Add_Hl_Sp, 2},
	{"LD A, (HL-)", 1, &Cpu::Ld_A_Hln, 2},
	{"DEC SP", 1, &Cpu::Dec_Sp, 2},
	{"INC A", 1, &Cpu::Inc_A, 1},
	{"DEC A", 1, &Cpu::Dec_A, 1},
	{"LD A, d8", 2, &Cpu::Ld_A, 2},
	{"CCF", 1, &Cpu::Ccf, 1},

	{"LD B, B", 1, &Cpu::Ld_B_B, 1}, /* 0x40 */
	{"LD B, C", 1, &Cpu::Ld_B_C, 1},
	{"LD B, D", 1, &Cpu::Ld_B_D, 1},
	{"LD B, E", 1, &Cpu::Ld_B_E, 1},
	{"LD B, H", 1, &Cpu::Ld_B_H, 1},
	{"LD B, L", 1, &Cpu::Ld_B_L, 1},
	{"LD B, (HL)", 1, &Cpu::Ld_B_Hlp, 2},
	{"LD B, A", 1, &Cpu::Ld_B_A, 1},
	{"LD C, B", 1, &Cpu::Ld_C_B, 1},
	{"LD C, C", 1, &Cpu::Ld_C_C, 1},
	{"LD C, D", 1, &Cpu::Ld_C_D, 1},
	{"LD C, E", 1, &Cpu::Ld_C_E, 1},
	{"LD C, H", 1, &Cpu::Ld_C_H, 1},
	{"LD C, L", 1, &Cpu::Ld_C_L, 1},
	{"LD C, (HL)", 1, &Cpu::Ld_C_Hlp, 2},
	{"LD C, A", 1, &Cpu::Ld_C_A, 1},

	{"LD D, B", 1, &Cpu::Ld_D_B, 1}, /* 0x50 */
	{"LD D, C", 1, &Cpu::Ld_D_C, 1},
	{"LD D, D", 1, &Cpu::Ld_D_D, 1},
	{"LD D, E", 1, &Cpu::Ld_D_E, 1},
	{"LD D, H", 1, &Cpu::Ld_D_H, 1},
	{"LD D, L", 1, &Cpu::Ld_D_L, 1},
	{"LD D, (HL)", 1, &Cpu::Ld_D_Hlp, 2},
	{"LD D, A", 1, &Cpu::Ld_D_A, 1},
	{"LD E, B", 1, &Cpu::Ld_E_B, 1},
	{"LD E, C", 1, &Cpu::Ld_E_C, 1},
	{"LD E, D", 1, &Cpu::Ld_E_D, 1},
	{"LD E, E", 1, &Cpu::Ld_E_E, 1},
	{"LD E, H", 1, &Cpu::Ld_E_H, 1},
	{"LD E, L", 1, &Cpu::Ld_E_L, 1},
	{"LD E, (HL)", 1, &Cpu::Ld_E_Hlp, 2},
	{"LD E, A", 1, &Cpu::Ld_E_A, 1},

	{"LD H, B", 1, &Cpu::Ld_H_B, 1}, /* 0x60 */
	{"LD H, C", 1, &Cpu::Ld_H_C, 1},
	{"LD H, D", 1, &Cpu::Ld_H_D, 1},
	{"LD H, E", 1, &Cpu::Ld_H_E, 1},
	{"LD H, H", 1, &Cpu::Ld_H_H, 1},
	{"LD H, L", 1, &Cpu::Ld_H_L, 1},
	{"LD H, (HL)", 1, &Cpu::Ld_H_Hlp, 2},
	{"LD H, A", 1, &Cpu::Ld_H_A, 1},
	{"LD L, B", 1, &Cpu::Ld_L_B, 1},
	{"LD L, C", 1, &Cpu::Ld_L_C, 1},
	{"LD L, D", 1, &Cpu::Ld_L_D, 1},
	{"LD L, E", 1, &Cpu::Ld_L_E, 1},
	{"LD L, H", 1, &Cpu::Ld_L_H, 1},
	{"LD L, L", 1, &Cpu::Ld_L_L, 1},
	{"LD L, (HL)", 1, &Cpu::Ld_L_Hlp, 2},
	{"LD L, A", 1, &Cpu::Ld_L_A, 1},

	{"LD (HL), B", 1, &Cpu::Ld_Hlp_B, 2}, /* 0x70 */
	{"LD (HL), C", 1, &Cpu::Ld_Hlp_C, 2},
	{"LD (HL), D", 1, &Cpu::Ld_Hlp_D, 2},
	{"LD (HL), E", 1, &Cpu::Ld_Hlp_E, 2},
	{"LD (HL), H", 1, &Cpu::Ld_Hlp_H, 2},
	{"LD (HL), L", 1, &Cpu::Ld_Hlp_L, 2},
	{"HALT", 1, &Cpu::Halt, 1},
	{"LD (HL), A", 1, &Cpu::Ld_Hlp_A, 2},
	{"LD A, B", 1, &Cpu::Ld_A_B, 1},
	{"LD A, C", 1, &Cpu::Ld_A_C, 1},
	{"LD A, D", 1, &Cpu::Ld_A_D, 1},
	{"LD A, E", 1, &Cpu::Ld_A_E, 1},
	{"LD A, H", 1, &Cpu::Ld_A_H, 1},
	{"LD A, L", 1, &Cpu::Ld_A_L, 1},
	{"LD A, (HL)", 1, &Cpu::Ld_A_Hlp, 2},
	{"LD A, A", 1, &Cpu::Ld_A_A, 1},

	{"ADD A, B", 1, &Cpu::Add_A_B, 1}, /* 0x80 */
	{"ADD A, C", 1, &Cpu::Add_A_C, 1},
	{"ADD A, D", 1, &Cpu::Add_A_D, 1},
	{"ADD A, E", 1, &Cpu::Add_A_E, 1},
	{"ADD A, H", 1, &Cpu::Add_A_H, 1},
	{"ADD A, L", 1, &Cpu::Add_A_L, 1},
	{"ADD A, (HL)", 1, &Cpu::Add_A_Hlp, 2},
	{"ADD A, A", 1, &Cpu::Add_A_A, 1},
	{"ADC A, B", 1, &Cpu::Adc_A_B, 1},
	{"ADC A, C", 1, &Cpu::Adc_A_C, 1},
	{"ADC A, D", 1, &Cpu::Adc_A_D, 1},
	{"ADC A, E", 1, &Cpu::Adc_A_E, 1},
	{"ADC A, H", 1, &Cpu::Adc_A_H, 1},
	{"ADC A, L", 1, &Cpu::Adc_A_L, 1},
	{"ADC A, (HL)", 1, &Cpu::Adc_A_Hlp, 2},
	{"ADC A, A", 1, &Cpu::Adc_A_A, 1},

	{"SUB B", 1, &Cpu::Sub_B, 1}, /* 0x90 */
	{"SUB C", 1, &Cpu::Sub_C, 1},
	{"SUB D", 1, &Cpu::Sub_D, 1},
	{"SUB E", 1, &Cpu::Sub_E, 1},
	{"SUB H", 1, &Cpu::Sub_H, 1},
	{"SUB L", 1, &Cpu::Sub_L, 1},
	{"SUB (HL)", 1, &Cpu::Sub_Hlp, 2},
	{"SUB A", 1, &Cpu::Sub_A, 1},
	{"SBC A, B", 1, &Cpu::Sbc_A_B, 1},
	{"SBC A, C", 1, &Cpu::Sbc_A_C, 1},
	{"SBC A, D", 1, &Cpu::Sbc_A_D, 1},
	{"SBC A, E", 1, &Cpu::Sbc_A_E, 1},
	{"SBC A, H", 1, &Cpu::Sbc_A_H, 1},
	{"SBC A, L", 1, &Cpu::Sbc_A_L, 1},
	{"SBC A, (HL)", 1, &Cpu::Sbc_A_Hlp, 2},
	{"SBC A, A", 1, &Cpu::Sbc_A_A, 1},

	{"AND B", 1, &Cpu::And_B, 1}, /* 0xA0 */
	{"AND C", 1, &Cpu::And_C, 1},
	{"AND D", 1, &Cpu::And_D, 1},
	{"AND E", 1, &Cpu::And_E, 1},
	{"AND H", 1, &Cpu::And_H, 1},
	{"AND L", 1, &Cpu::And_L, 1},
	{"AND (HL)", 1, &Cpu::And_Hlp, 2},
	{"AND A", 1, &Cpu::And_A, 1},
	{"XOR B", 1, &Cpu::Xor_B, 1},
	{"XOR C", 1, &Cpu::Xor_C, 1},
	{"XOR D", 1, &Cpu::Xor_D, 1},
	{"XOR E", 1, &Cpu::Xor_E, 1},
	{"XOR H", 1, &Cpu::Xor_H, 1},
	{"XOR L", 1, &Cpu::Xor_L, 1},
	{"XOR (HL)", 1, &Cpu::Xor_Hlp, 2},
	{"XOR A", 1, &Cpu::Xor_A, 1},

	{"OR B", 1, &Cpu::Or_B, 1}, /* 0xB0 */
	{"OR C", 1, &Cpu::Or_C, 1},
	{"OR D", 1, &Cpu::Or_D, 1},
	{"OR E", 1, &Cpu::Or_E, 1},
	{"OR H", 1, &Cpu::Or_H, 1},
	{"OR L", 1, &Cpu::Or_L, 1},
	{"OR (HL)", 1, &Cpu::Or_Hlp, 2},
	{"OR A", 1, &Cpu::Or_A, 1},
	{"CP B", 1, &Cpu::Cp_B, 1},
	{"CP C", 1, &Cpu::Cp_C, 1},
	{"CP D", 1, &Cpu::Cp_D, 1},
	{"CP E", 1, &Cpu::Cp_E, 1},
	{"CP H", 1, &Cpu::Cp_H, 1},
	{"CP L", 1, &Cpu::Cp_L, 1},
	{"CP (HL)", 1, &Cpu::Cp_Hlp, 2},
	{"CP A", 1, &Cpu::Cp_A, 1},

	{"RET NZ", 1, &Cpu::Ret_Nz, 5}, //8  /* 0xC0 */
	{"POP BC", 1, &Cpu::Pop_Bc, 3},
	{"JP NZ, a16", 3, &Cpu::Jp_Nz, 4},//3
	{"JP a16", 3, &Cpu::Jp, 4},
	{"CALL NZ, a16", 3, &Cpu::Call_Nz, 6}, // 3
	{"PUSH BC", 1, &Cpu::Push_Bc, 4},
	{"ADD  A,d8", 2, &Cpu::Add_A, 2},
	{"RST 00H", 1, &Cpu::Rst_00H, 4},
	{"RET Z", 1, &Cpu::Ret_Z, 4}, // 2
	{"RET", 1, &Cpu::Ret, 4},
	{"JP Z, a16", 3, &Cpu::Jp_Z, 4}, //3
	{"PREFIX CB", 1, &Cpu::Prefix_Cb, 1},
	{"CALL Z, a16", 3, &Cpu::Call_Z, 6}, //3
	{"CALL a16", 3, &Cpu::Call, 6},
	{"ADC  A, d8", 2, &Cpu::Adc, 2},
	{"RST 08H", 1, &Cpu::Rst_08H, 4},

	{"RET NC", 1, &Cpu::Ret_Nc, 5}, // 2 /* 0xD0 */
	{"POP DE", 1, &Cpu::Pop_De, 3},
	{"JP NC, a16", 3, &Cpu::Jp_Nc, 4}, // 3
	{"NOT IMPLEMENTED", 1, &Cpu::default_operator, 1},
	{"CALL NC, a16", 3, &Cpu::Call_Nc, 6}, // 3
	{"PUSH DE", 1, &Cpu::Push_De, 4},
	{"SUB  d8", 1, &Cpu::Sub, 2},
	{"RST 10H", 1, &Cpu::Rst_10H, 4},
	{"RET C", 1, &Cpu::Ret_C, 5}, // 2
	{"RETI", 1, &Cpu::Reti, 4}, // 3
	{"JP C, a16", 3, &Cpu::Jp_c, 4}, // 3
	{"NOT IMPLEMENTED", 1, &Cpu::default_operator, 1},
	{"CALL C, a16", 3, &Cpu::Call_C, 6}, // 3
	{"NOT IMPLEMENTED", 1, &Cpu::default_operator, 1},
	{"SBC  A, d8", 2, &Cpu::Sbc, 2},
	{"RST 18H", 1, &Cpu::Rst_18H, 3},

	{"LDH a8, A", 2, &Cpu::Ldh_A, 3}, /* 0xE0 */
	{"POP HL", 1, &Cpu::Pop_Hl, 3},
	{"LD (C), A", 1, &Cpu::Ld_Cp, 2},
	{"NOT IMPLEMENTED", 1, &Cpu::default_operator, 1},
	{"NOT IMPLEMENTED", 1, &Cpu::default_operator, 1},
	{"PUSH HL", 1, &Cpu::Push_Hl, 4},
	{"AND  d8", 2, &Cpu::And, 2},
	{"RST 20H", 1, &Cpu::Rst_20H, 4},
	{"ADD SP, r8", 2, &Cpu::Add_SP, 4},
	{"JP (HL)", 1, &Cpu::Jp_Hlp, 1},
	{"LD (a16), A", 3, &Cpu::Ld_A, 4},
	{"NOT IMPLEMENTED", 1, &Cpu::default_operator, 1},
	{"NOT IMPLEMENTED", 1, &Cpu::default_operator, 1},
	{"NOT IMPLEMENTED", 1, &Cpu::default_operator, 1},
	{"XOR  d8", 2, &Cpu::Xor, 2},
	{"RST 28H", 1, &Cpu::Rst_28H, 4},

	{"LDH a8, A", 2, &Cpu::Ldh_a_A, 3}, /* 0xF0 */
	{"POP AF", 1, &Cpu::Pop_Af, 3},
	{"LD A, (C)", 1, &Cpu::Ld_A_Cp, 2},
	{"DI", 1, &Cpu::Di, 1},
	{"NOT IMPLEMENTED", 1, &Cpu::default_operator, 1},
	{"PUSH AF", 1, &Cpu::Push_Af, 4},
	{"OR  d8", 2, &Cpu::Or, 2},
	{"RST 30H", 1, &Cpu::Rst_30H, 4},
	{"LD HL, SP+r8", 2, &Cpu::Ld_HL_SP, 3},
	{"LD SP, HL", 1, &Cpu::Ld_Sp_Hl, 2},
	{"LD A, (a16)", 3, &Cpu::Ld_A, 4},
	{"EI", 1, &Cpu::Ei, 1},
	{"NOT IMPLEMENTED", 1, &Cpu::default_operator, 1},
	{"NOT IMPLEMENTED", 1, &Cpu::default_operator, 1},
	{"CP  d8", 2, &Cpu::Cp, 2},
	{"RST 38H", 1, &Cpu::Rst_38H, 4},
};
