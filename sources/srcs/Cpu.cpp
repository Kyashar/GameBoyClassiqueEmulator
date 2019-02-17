/*
** EPITECH PROJECT, 2019
** Epitech scolarship project (4 years remaining)
** File description:
**      Made on 2019/01 by ebernard
*/

#include <iomanip>
#include <iostream>
#include "Cpu.hpp"

emulator::Cpu::Cpu(std::vector<uint8_t> &instructions) :
	_register{{{{0, 0}}}, {{{0, 0}}}, {{{0, 0}}}, {{{0, 0}}}, {{{0, 0}}}, 0, 0},
	_timer{0, 0}, _gpu(_memory), _read(true)
{
	_register.pc = 0;
	_register.sp = 0;
	_memory.loadRom(instructions);
	_romSize = instructions.size();

	_gpu.initWindow();
}

bool emulator::Cpu::gotSomethingToRead() const
{
	return _read;
}

void emulator::Cpu::disassemblyRom()
{
	auto rom = _memory.getRom();
}

void emulator::Cpu::readInstruction()
{
	uint8_t cData = 0;
	uint16_t sData = 0;
	auto tmp = _register.pc;
	bool readline = false;

	if (_register.pc == 0x1b0)
		readline = true;

	_register.t = 0;
	_register.m = 0;
	if (managedInstruction[_memory[_register.pc]]._length == 2) {
		cData = _memory[_register.pc + 1];
		sData = cData;
	} else if (managedInstruction[_memory[_register.pc]]._length == 3)
		sData = _memory.getShort(_register.pc + 1);
	if (readline)
		std::cout << managedInstruction[_memory[_register.pc]]._name << std::endl;
	if (managedInstruction[_memory[tmp]]._length > 1 && readline)
		std::cout << std::hex << "args :" << sData << std::endl;

	_register.t += managedInstruction[_memory[_register.pc]]._timer;
	_register.m = _register.t * 4;
	(*this.*managedInstruction[_memory[_register.pc]]._instruction)(sData);

	if (tmp == _register.pc)
		_register.pc += managedInstruction[_memory[tmp]]._length;
	_gpu.put(_register.t);
	std::string input_line;
	if (readline) {
		std::cout << std::hex << _register << std::endl;
		std::cout << std::hex << _memory.getGpuRegister() << std::endl;
		std::cout << "y: " << (_memory.getGpuRegister().getDisplay().y / 8) << std::endl;
		std::cout << "x: " << (_memory.getGpuRegister().getDisplay().x / 8) << std::endl;
//		_memory.dumpMemory(0x9800, 0x9BFF);
//		std::cout << std::endl;
//		_memory.dumpMemory(0x8000, 0x8000 + 256 * 16);
//		getline(std::cin, input_line);
	}
}

std::ostream &operator<<(std::ostream &os, const emulator::Cpu::instructionInfos &infos)
{
	os << infos._name;
	return os;
}

std::vector<emulator::Cpu::instructionInfos> emulator::Cpu::managedInstruction = {
	{"NOP", 1, &Cpu::Nop, 1}, /* 0x00 */
	{"LD BC, ction Findd16", 3, &Cpu::Ld_BC, 3},
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
	{"PREFIX CB", 2, &Cpu::Prefix_Cb, 1},
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

	{"LDH (a8), A", 2, &Cpu::Ldh_a_A, 3}, /* 0xE0 */
	{"POP HL", 1, &Cpu::Pop_Hl, 3},
	{"LD (C), A", 1, &Cpu::Ld_Cp, 2},
	{"NOT IMPLEMENTED", 1, &Cpu::default_operator, 1},
	{"NOT IMPLEMENTED", 1, &Cpu::default_operator, 1},
	{"PUSH HL", 1, &Cpu::Push_Hl, 4},
	{"AND  d8", 2, &Cpu::And, 2},
	{"RST 20H", 1, &Cpu::Rst_20H, 4},
	{"ADD SP, r8", 2, &Cpu::Add_SP, 4},
	{"JP (HL)", 1, &Cpu::Jp_Hlp, 1},
	{"LD (a16), A", 3, &Cpu::Ld_a_A, 4},
	{"NOT IMPLEMENTED", 1, &Cpu::default_operator, 1},
	{"NOT IMPLEMENTED", 1, &Cpu::default_operator, 1},
	{"NOT IMPLEMENTED", 1, &Cpu::default_operator, 1},
	{"XOR  d8", 2, &Cpu::Xor, 2},
	{"RST 28H", 1, &Cpu::Rst_28H, 4},

	{"LDH A, (a8)", 2, &Cpu::Ldh_A_a, 3}, /* 0xF0 */
	{"POP AF", 1, &Cpu::Pop_Af, 3},
	{"LD A, (C)", 1, &Cpu::Ld_A_Cp, 2},
	{"DI", 1, &Cpu::Di, 1},
	{"NOT IMPLEMENTED", 1, &Cpu::default_operator, 1},
	{"PUSH AF", 1, &Cpu::Push_Af, 4},
	{"OR  d8", 2, &Cpu::Or, 2},
	{"RST 30H", 1, &Cpu::Rst_30H, 4},
	{"LD HL, SP+r8", 2, &Cpu::Ld_HL_SP, 3},
	{"LD SP, HL", 1, &Cpu::Ld_Sp_Hl, 2},
	{"LD A, (a16)", 3, &Cpu::Ld_A_p, 4},
	{"EI", 1, &Cpu::Ei, 1},
	{"NOT IMPLEMENTED", 1, &Cpu::default_operator, 1},
	{"NOT IMPLEMENTED", 1, &Cpu::default_operator, 1},
	{"CP  d8", 2, &Cpu::Cp, 2},
	{"RST 38H", 1, &Cpu::Rst_38H, 4},
};

std::vector<emulator::Cpu::prefixInfos> emulator::Cpu::prefixInstruction = {
	{"RLC B", &Cpu::RLC_B}, /* 0x00 */
	{"RLC C", &Cpu::RLC_C},
	{"RLC D", &Cpu::RLC_D},
	{"RLC E", &Cpu::RLC_E},
	{"RLC H", &Cpu::RLC_H},
	{"RLC L", &Cpu::RLC_L},
	{"RLC (HL)", &Cpu::RLC_HLp},
	{"RLC A", &Cpu::RLC_A},
	{"RRC B", &Cpu::RRC_B},
	{"RRC C", &Cpu::RRC_C},
	{"RRC D", &Cpu::RRC_D},
	{"RRC E", &Cpu::RRC_E},
	{"RRC H", &Cpu::RRC_H},
	{"RRC L", &Cpu::RRC_L},
	{"RRC (HL)", &Cpu::RRC_HLp},
	{"RRC A", &Cpu::RRC_A},

	{"RL B", &Cpu::RL_B}, /* 0x00 */
	{"RL C", &Cpu::RL_C},
	{"RL D", &Cpu::RL_D},
	{"RL E", &Cpu::RL_E},
	{"RL H", &Cpu::RL_H},
	{"RL L", &Cpu::RL_L},
	{"RL (HL)", &Cpu::RL_HLp},
	{"RL A", &Cpu::RL_A},
	{"RR B", &Cpu::RR_B},
	{"RR C", &Cpu::RR_C},
	{"RR D", &Cpu::RR_D},
	{"RR E", &Cpu::RR_E},
	{"RR H", &Cpu::RR_H},
	{"RR L", &Cpu::RR_L},
	{"RR (HL)", &Cpu::RR_HLp},
	{"RR A", &Cpu::RR_A},

	{"SLA B", &Cpu::SLA_B}, /* 0x00 */
	{"SLA C", &Cpu::SLA_C},
	{"SLA D", &Cpu::SLA_D},
	{"SLA E", &Cpu::SLA_E},
	{"SLA H", &Cpu::SLA_H},
	{"SLA L", &Cpu::SLA_L},
	{"SLA (HL)", &Cpu::SLA_HLp},
	{"SLA A", &Cpu::SLA_A},
	{"SRA B", &Cpu::SRA_B},
	{"SRA C", &Cpu::SRA_C},
	{"SRA D", &Cpu::SRA_D},
	{"SRA E", &Cpu::SRA_E},
	{"SRA H", &Cpu::SRA_H},
	{"SRA L", &Cpu::SRA_L},
	{"SRA (HL)", &Cpu::SRA_HLp},
	{"SRA A", &Cpu::SRA_A},

	{"SWAP B", &Cpu::SWAP_B}, /* 0x00 */
	{"SWAP C", &Cpu::SWAP_C},
	{"SWAP D", &Cpu::SWAP_D},
	{"SWAP E", &Cpu::SWAP_E},
	{"SWAP H", &Cpu::SWAP_H},
	{"SWAP L", &Cpu::SWAP_L},
	{"SWAP (HL)", &Cpu::SWAP_HLp},
	{"SWAP A", &Cpu::SWAP_A},
	{"SRL B", &Cpu::SRL_B},
	{"SRL C", &Cpu::SRL_C},
	{"SRL D", &Cpu::SRL_D},
	{"SRL E", &Cpu::SRL_E},
	{"SRL H", &Cpu::SRL_H},
	{"SRL L", &Cpu::SRL_L},
	{"SRL (HL)", &Cpu::SRL_HLp},
	{"SRL A", &Cpu::SRL_A},

	{"BIT 0, B", &Cpu::BIT_0_B}, /* 0x00 */
	{"BIT 0, C", &Cpu::BIT_0_C},
	{"BIT 0, D", &Cpu::BIT_0_D},
	{"BIT 0, E", &Cpu::BIT_0_E},
	{"BIT 0, H", &Cpu::BIT_0_H},
	{"BIT 0, L", &Cpu::BIT_0_L},
	{"BIT 0, (HL)", &Cpu::BIT_0_Hlp},
	{"BIT 0, A", &Cpu::BIT_0_A},
	{"BIT 1, B", &Cpu::BIT_1_B},
	{"BIT 1, C", &Cpu::BIT_1_C},
	{"BIT 1, D", &Cpu::BIT_1_D},
	{"BIT 1, E", &Cpu::BIT_1_E},
	{"BIT 1, H", &Cpu::BIT_1_H},
	{"BIT 1, L", &Cpu::BIT_1_L},
	{"BIT 1, (HL)", &Cpu::BIT_1_Hlp},
	{"BIT 1, A", &Cpu::BIT_1_A},

	{"BIT 2, B", &Cpu::BIT_2_B}, /* 0x00 */
	{"BIT 2, C", &Cpu::BIT_2_C},
	{"BIT 2, D", &Cpu::BIT_2_D},
	{"BIT 2, E", &Cpu::BIT_2_E},
	{"BIT 2, H", &Cpu::BIT_2_H},
	{"BIT 2, L", &Cpu::BIT_2_L},
	{"BIT 2, (HL)", &Cpu::BIT_2_Hlp},
	{"BIT 2, A", &Cpu::BIT_2_A},
	{"BIT 3, B", &Cpu::BIT_3_B},
	{"BIT 3, C", &Cpu::BIT_3_C},
	{"BIT 3, D", &Cpu::BIT_3_D},
	{"BIT 3, E", &Cpu::BIT_3_E},
	{"BIT 3, H", &Cpu::BIT_3_H},
	{"BIT 3, L", &Cpu::BIT_3_L},
	{"BIT 3, (HL)", &Cpu::BIT_3_Hlp},
	{"BIT 3, A", &Cpu::BIT_3_A},

	{"BIT 4, B", &Cpu::BIT_4_B}, /* 0x00 */
	{"BIT 4, C", &Cpu::BIT_4_C},
	{"BIT 4, D", &Cpu::BIT_4_D},
	{"BIT 4, E", &Cpu::BIT_4_E},
	{"BIT 4, H", &Cpu::BIT_4_H},
	{"BIT 4, L", &Cpu::BIT_4_L},
	{"BIT 4, (HL)", &Cpu::BIT_4_Hlp},
	{"BIT 4, A", &Cpu::BIT_4_A},
	{"BIT 5, B", &Cpu::BIT_5_B},
	{"BIT 5, C", &Cpu::BIT_5_C},
	{"BIT 5, D", &Cpu::BIT_5_D},
	{"BIT 5, E", &Cpu::BIT_5_E},
	{"BIT 5, H", &Cpu::BIT_5_H},
	{"BIT 5, L", &Cpu::BIT_5_L},
	{"BIT 5, (HL)", &Cpu::BIT_5_Hlp},
	{"BIT 5, A", &Cpu::BIT_5_A},

	{"BIT 6, B", &Cpu::BIT_6_B}, /* 0x00 */
	{"BIT 6, C", &Cpu::BIT_6_C},
	{"BIT 6, D", &Cpu::BIT_6_D},
	{"BIT 6, E", &Cpu::BIT_6_E},
	{"BIT 6, H", &Cpu::BIT_6_H},
	{"BIT 6, L", &Cpu::BIT_6_L},
	{"BIT 6, (HL)", &Cpu::BIT_6_Hlp},
	{"BIT 6, A", &Cpu::BIT_6_A},
	{"BIT 7, B", &Cpu::BIT_7_B},
	{"BIT 7, C", &Cpu::BIT_7_C},
	{"BIT 7, D", &Cpu::BIT_7_D},
	{"BIT 7, E", &Cpu::BIT_7_E},
	{"BIT 7, H", &Cpu::BIT_7_H},
	{"BIT 7, L", &Cpu::BIT_7_L},
	{"BIT 7, (HL)", &Cpu::BIT_7_Hlp},
	{"BIT 7, A", &Cpu::BIT_7_A},

	{"RES 0, B", &Cpu::RES_0_B}, /* 0x00 */
	{"RES 0, C", &Cpu::RES_0_C},
	{"RES 0, D", &Cpu::RES_0_D},
	{"RES 0, E", &Cpu::RES_0_E},
	{"RES 0, H", &Cpu::RES_0_H},
	{"RES 0, L", &Cpu::RES_0_L},
	{"RES 0, (HL)", &Cpu::RES_0_Hlp},
	{"RES 0, A", &Cpu::RES_0_A},
	{"RES 1, B", &Cpu::RES_1_B},
	{"RES 1, C", &Cpu::RES_1_C},
	{"RES 1, D", &Cpu::RES_1_D},
	{"RES 1, E", &Cpu::RES_1_E},
	{"RES 1, H", &Cpu::RES_1_H},
	{"RES 1, L", &Cpu::RES_1_L},
	{"RES 1, (HL)", &Cpu::RES_1_Hlp},
	{"RES 1, A", &Cpu::RES_1_A},

	{"RES 2, B", &Cpu::RES_2_B}, /* 0x00 */
	{"RES 2, C", &Cpu::RES_2_C},
	{"RES 2, D", &Cpu::RES_2_D},
	{"RES 2, E", &Cpu::RES_2_E},
	{"RES 2, H", &Cpu::RES_2_H},
	{"RES 2, L", &Cpu::RES_2_L},
	{"RES 2, (HL)", &Cpu::RES_2_Hlp},
	{"RES 2, A", &Cpu::RES_2_A},
	{"RES 3, B", &Cpu::RES_3_B},
	{"RES 3, C", &Cpu::RES_3_C},
	{"RES 3, D", &Cpu::RES_3_D},
	{"RES 3, E", &Cpu::RES_3_E},
	{"RES 3, H", &Cpu::RES_3_H},
	{"RES 3, L", &Cpu::RES_3_L},
	{"RES 3, (HL)", &Cpu::RES_3_Hlp},
	{"RES 3, A", &Cpu::RES_3_A},

	{"RES 4, B", &Cpu::RES_4_B}, /* 0x00 */
	{"RES 4, C", &Cpu::RES_4_C},
	{"RES 4, D", &Cpu::RES_4_D},
	{"RES 4, E", &Cpu::RES_4_E},
	{"RES 4, H", &Cpu::RES_4_H},
	{"RES 4, L", &Cpu::RES_4_L},
	{"RES 4, (HL)", &Cpu::RES_4_Hlp},
	{"RES 4, A", &Cpu::RES_4_A},
	{"RES 5, B", &Cpu::RES_5_B},
	{"RES 5, C", &Cpu::RES_5_C},
	{"RES 5, D", &Cpu::RES_5_D},
	{"RES 5, E", &Cpu::RES_5_E},
	{"RES 5, H", &Cpu::RES_5_H},
	{"RES 5, L", &Cpu::RES_5_L},
	{"RES 5, (HL)", &Cpu::RES_5_Hlp},
	{"RES 5, A", &Cpu::RES_5_A},

	{"RES 6, B", &Cpu::RES_6_B}, /* 0x00 */
	{"RES 6, C", &Cpu::RES_6_C},
	{"RES 6, D", &Cpu::RES_6_D},
	{"RES 6, E", &Cpu::RES_6_E},
	{"RES 6, H", &Cpu::RES_6_H},
	{"RES 6, L", &Cpu::RES_6_L},
	{"RES 6, (HL)", &Cpu::RES_6_Hlp},
	{"RES 6, A", &Cpu::RES_6_A},
	{"RES 7, B", &Cpu::RES_7_B},
	{"RES 7, C", &Cpu::RES_7_C},
	{"RES 7, D", &Cpu::RES_7_D},
	{"RES 7, E", &Cpu::RES_7_E},
	{"RES 7, H", &Cpu::RES_7_H},
	{"RES 7, L", &Cpu::RES_7_L},
	{"RES 7, (HL)", &Cpu::RES_7_Hlp},
	{"RES 7, A", &Cpu::RES_7_A},

	{"SET 0, B", &Cpu::SET_0_B}, /* 0x00 */
	{"SET 0, C", &Cpu::SET_0_C},
	{"SET 0, D", &Cpu::SET_0_D},
	{"SET 0, E", &Cpu::SET_0_E},
	{"SET 0, H", &Cpu::SET_0_H},
	{"SET 0, L", &Cpu::SET_0_L},
	{"SET 0, (HL)", &Cpu::SET_0_Hlp},
	{"SET 0, A", &Cpu::SET_0_A},
	{"SET 1, B", &Cpu::SET_1_B},
	{"SET 1, C", &Cpu::SET_1_C},
	{"SET 1, D", &Cpu::SET_1_D},
	{"SET 1, E", &Cpu::SET_1_E},
	{"SET 1, H", &Cpu::SET_1_H},
	{"SET 1, L", &Cpu::SET_1_L},
	{"SET 1, (HL)", &Cpu::SET_1_Hlp},
	{"SET 1, A", &Cpu::SET_1_A},

	{"SET 2, B", &Cpu::SET_2_B}, /* 0x00 */
	{"SET 2, C", &Cpu::SET_2_C},
	{"SET 2, D", &Cpu::SET_2_D},
	{"SET 2, E", &Cpu::SET_2_E},
	{"SET 2, H", &Cpu::SET_2_H},
	{"SET 2, L", &Cpu::SET_2_L},
	{"SET 2, (HL)", &Cpu::SET_2_Hlp},
	{"SET 2, A", &Cpu::SET_2_A},
	{"SET 3, B", &Cpu::SET_3_B},
	{"SET 3, C", &Cpu::SET_3_C},
	{"SET 3, D", &Cpu::SET_3_D},
	{"SET 3, E", &Cpu::SET_3_E},
	{"SET 3, H", &Cpu::SET_3_H},
	{"SET 3, L", &Cpu::SET_3_L},
	{"SET 3, (HL)", &Cpu::SET_3_Hlp},
	{"SET 3, A", &Cpu::SET_3_A},

	{"SET 4, B", &Cpu::SET_4_B}, /* 0x00 */
	{"SET 4, C", &Cpu::SET_4_C},
	{"SET 4, D", &Cpu::SET_4_D},
	{"SET 4, E", &Cpu::SET_4_E},
	{"SET 4, H", &Cpu::SET_4_H},
	{"SET 4, L", &Cpu::SET_4_L},
	{"SET 4, (HL)", &Cpu::SET_4_Hlp},
	{"SET 4, A", &Cpu::SET_4_A},
	{"SET 5, B", &Cpu::SET_5_B},
	{"SET 5, C", &Cpu::SET_5_C},
	{"SET 5, D", &Cpu::SET_5_D},
	{"SET 5, E", &Cpu::SET_5_E},
	{"SET 5, H", &Cpu::SET_5_H},
	{"SET 5, L", &Cpu::SET_5_L},
	{"SET 5, (HL)", &Cpu::SET_5_Hlp},
	{"SET 5, A", &Cpu::SET_5_A},

	{"SET 6, B", &Cpu::SET_6_B}, /* 0x00 */
	{"SET 6, C", &Cpu::SET_6_C},
	{"SET 6, D", &Cpu::SET_6_D},
	{"SET 6, E", &Cpu::SET_6_E},
	{"SET 6, H", &Cpu::SET_6_H},
	{"SET 6, L", &Cpu::SET_6_L},
	{"SET 6, (HL)", &Cpu::SET_6_Hlp},
	{"SET 6, A", &Cpu::SET_6_A},
	{"SET 7, B", &Cpu::SET_7_B},
	{"SET 7, C", &Cpu::SET_7_C},
	{"SET 7, D", &Cpu::SET_7_D},
	{"SET 7, E", &Cpu::SET_7_E},
	{"SET 7, H", &Cpu::SET_7_H},
	{"SET 7, L", &Cpu::SET_7_L},
	{"SET 7, (HL)", &Cpu::SET_7_Hlp},
	{"SET 7, A", &Cpu::SET_7_A}
};
