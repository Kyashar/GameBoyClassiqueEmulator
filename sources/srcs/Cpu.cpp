/*
** EPITECH PROJECT, 2019
** Epitech scolarship project (4 years remaining)
** File description:
**      Made on 2019/01 by ebernard
*/

#include <iostream>
#include "Cpu.hpp"

emulator::Cpu::Cpu(std::vector<uint8_t> &instructions) :
	_read(true)
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
	char cData;
	short sData;

//	std::cout << (int)_instruction[_register.pc] << std::endl;
	std::cout << managedInstruction[_memory[_register.pc]] << std::endl;

//	if (managedInstruction[_memory[_register.pc]]._length == 2) {
//		cData = *((char *) (&_memory[_register.pc]));
//		std::cout << "cData: " << std::to_string((int)cData) << std::endl;
//	} else if (managedInstruction[_memory[_register.pc]]._length == 3) {
//		sData = _memory.getShort(_register.pc);
//		std::cout << "sData: " << std::to_string((int)sData) << std::endl;
//	}
	_register.pc += managedInstruction[_memory[_register.pc]]._length;
	if (_register.pc > _romSize)
		exit(1);
	_gpu.put(managedInstruction[_memory[_register.pc]]._timer);
}

void emulator::Cpu::Nop()
{

}

void emulator::Cpu::Ld()
{

}

std::ostream &operator<<(std::ostream &os, const emulator::Cpu::instructionInfos &infos)
{
	os << infos._name;
	return os;
}

std::vector<emulator::Cpu::instructionInfos> emulator::Cpu::managedInstruction = {
	{"NOP", 1, &Cpu::Nop, 1}, /* 0x00 */
	{"LD BC, d16", 3, nullptr, 1},
	{"LD (BC), A", 1, nullptr, 1},
	{"INC BC", 1, nullptr, 1},
	{"INC B", 1, nullptr, 1},
	{"DEC B", 1, nullptr, 1},
	{"LD B, d8", 2, nullptr, 1},
	{"RLCA", 1, nullptr, 1},
	{"LD (a16), SP", 3, nullptr, 1},
	{"ADD HL, BC", 1, nullptr, 1},
	{"LD A, (BC)", 1, nullptr, 1},
	{"DEC BC", 1, nullptr, 1},
	{"INC C", 1, nullptr, 1},
	{"DEC C", 1, nullptr, 1},
	{"LD C, d8", 2, nullptr, 1},
	{"RRCA", 1, nullptr, 1},

	{"STOP 0", 1, nullptr, 1}, /* 0x10 */
	{"LD DE, d16", 3, nullptr, 1},
	{"LD (DE), A", 1, nullptr, 1},
	{"INC DE", 1, nullptr, 1},
	{"INC D", 1, nullptr, 1},
	{"DEC D", 1, nullptr, 1},
	{"LD D, d8", 2, nullptr, 1},
	{"RLA", 1, nullptr, 1},
	{"JR r8", 2, nullptr, 1},
	{"ADD HL, DE", 1, nullptr, 1},
	{"LD A, (DE)", 1, nullptr, 1},
	{"DEC DE", 1, nullptr, 1},
	{"INC E", 1, nullptr, 1},
	{"DEC E", 1, nullptr, 1},
	{"LD E, d8", 2, nullptr, 1},
	{"RRA", 1, nullptr, 1},

	{"JR NZ, r8", 2, nullptr, 1}, /* 0x20 */
	{"LD HL, d16", 3, nullptr, 1},
	{"LD (HL+), A", 1, nullptr, 1},
	{"INC HL", 1, nullptr, 1},
	{"INC H", 1, nullptr, 1},
	{"DEC H", 1, nullptr, 1},
	{"LD H, d8", 2, nullptr, 1},
	{"DAA", 1, nullptr, 1},
	{"JR Z, r8", 2, nullptr, 1},
	{"ADD HL, HL", 1, nullptr, 1},
	{"LD A, (HL+)", 1, nullptr, 1},
	{"DEC HL", 1, nullptr, 1},
	{"INC L", 1, nullptr, 1},
	{"DEC L", 1, nullptr, 1},
	{"LD L, d8", 2, nullptr, 1},
	{"CPL", 1, nullptr, 1},

	{"JR NC, r8", 2, nullptr, 1}, /* 0x30 */
	{"LD SP, d16", 3, nullptr, 1},
	{"LD (HL-), A", 1, nullptr, 1},
	{"INC SP", 1, nullptr, 1},
	{"INC (HL)", 1, nullptr, 1},
	{"DEC (HL)", 1, nullptr, 1},
	{"LD (HL), d8", 2, nullptr, 1},
	{"SCF", 1, nullptr, 1},
	{"JR C, r8", 2, nullptr, 1},
	{"ADD HL, SP", 1, nullptr, 1},
	{"LD A, (HL-)", 1, nullptr, 1},
	{"DEC SP", 1, nullptr, 1},
	{"INC A", 1, nullptr, 1},
	{"DEC A", 1, nullptr, 1},
	{"LD A, d8", 2, nullptr, 1},
	{"CCF", 1, nullptr, 1},

	{"LD B, B", 1, nullptr, 1}, /* 0x40 */
	{"LD B, C", 1, nullptr, 1},
	{"LD B, D", 1, nullptr, 1},
	{"LD B, E", 1, nullptr, 1},
	{"LD B, H", 1, nullptr, 1},
	{"LD B, L", 1, nullptr, 1},
	{"LD B, (HL)", 1, nullptr, 1},
	{"LD B, A", 1, nullptr, 1},
	{"LD C, B", 1, nullptr, 1},
	{"LD C, C", 1, nullptr, 1},
	{"LD C, D", 1, nullptr, 1},
	{"LD C, E", 1, nullptr, 1},
	{"LD C, H", 1, nullptr, 1},
	{"LD C, L", 1, nullptr, 1},
	{"LD C, (HL)", 1, nullptr, 1},
	{"LD C, A", 1, nullptr, 1},

	{"LD D, B", 1, nullptr, 1}, /* 0x50 */
	{"LD D, C", 1, nullptr, 1},
	{"LD D, D", 1, nullptr, 1},
	{"LD D, E", 1, nullptr, 1},
	{"LD D, H", 1, nullptr, 1},
	{"LD D, L", 1, nullptr, 1},
	{"LD D, (HL)", 1, nullptr, 1},
	{"LD D, A", 1, nullptr, 1},
	{"LD E, B", 1, nullptr, 1},
	{"LD E, C", 1, nullptr, 1},
	{"LD E, D", 1, nullptr, 1},
	{"LD E, E", 1, nullptr, 1},
	{"LD E, H", 1, nullptr, 1},
	{"LD E, L", 1, nullptr, 1},
	{"LD E, (HL)", 1, nullptr, 1},
	{"LD E, A", 1, nullptr, 1},

	{"LD H, B", 1, nullptr, 1}, /* 0x60 */
	{"LD H, C", 1, nullptr, 1},
	{"LD H, D", 1, nullptr, 1},
	{"LD H, E", 1, nullptr, 1},
	{"LD H, H", 1, nullptr, 1},
	{"LD H, L", 1, nullptr, 1},
	{"LD H, (HL)", 1, nullptr, 1},
	{"LD H, A", 1, nullptr, 1},
	{"LD L, B", 1, nullptr, 1},
	{"LD L, C", 1, nullptr, 1},
	{"LD L, D", 1, nullptr, 1},
	{"LD L, E", 1, nullptr, 1},
	{"LD L, H", 1, nullptr, 1},
	{"LD L, L", 1, nullptr, 1},
	{"LD L, (HL)", 1, nullptr, 1},
	{"LD L, A", 1, nullptr, 1},

	{"LD (HL), B", 1, nullptr, 1}, /* 0x70 */
	{"LD (HL), C", 1, nullptr, 1},
	{"LD (HL), D", 1, nullptr, 1},
	{"LD (HL), E", 1, nullptr, 1},
	{"LD (HL), H", 1, nullptr, 1},
	{"LD (HL), L", 1, nullptr, 1},
	{"HALT", 1, nullptr, 1},
	{"LD (HL), A", 1, nullptr, 1},
	{"LD A, B", 1, nullptr, 1},
	{"LD A, C", 1, nullptr, 1},
	{"LD A, D", 1, nullptr, 1},
	{"LD A, E", 1, nullptr, 1},
	{"LD A, H", 1, nullptr, 1},
	{"LD A, L", 1, nullptr, 1},
	{"LD A, (HL)", 1, nullptr, 1},
	{"LD A, A", 1, nullptr, 1},

	{"ADD A, B", 1, nullptr, 1}, /* 0x80 */
	{"ADD A, C", 1, nullptr, 1},
	{"ADD A, D", 1, nullptr, 1},
	{"ADD A, E", 1, nullptr, 1},
	{"ADD A, H", 1, nullptr, 1},
	{"ADD A, L", 1, nullptr, 1},
	{"ADD A, (HL)", 1, nullptr, 1},
	{"ADD A, A", 1, nullptr, 1},
	{"ADC A, B", 1, nullptr, 1},
	{"ADC A, C", 1, nullptr, 1},
	{"ADC A, D", 1, nullptr, 1},
	{"ADC A, E", 1, nullptr, 1},
	{"ADC A, H", 1, nullptr, 1},
	{"ADC A, L", 1, nullptr, 1},
	{"ADC A, (HL)", 1, nullptr, 1},
	{"ADC A, A", 1, nullptr, 1},

	{"SUB B", 1, nullptr, 1}, /* 0x90 */
	{"SUB C", 1, nullptr, 1},
	{"SUB D", 1, nullptr, 1},
	{"SUB E", 1, nullptr, 1},
	{"SUB H", 1, nullptr, 1},
	{"SUB L", 1, nullptr, 1},
	{"SUB (HL)", 1, nullptr, 1},
	{"SUB A", 1, nullptr, 1},
	{"SBC A, B", 1, nullptr, 1},
	{"SBC A, C", 1, nullptr, 1},
	{"SBC A, D", 1, nullptr, 1},
	{"SBC A, E", 1, nullptr, 1},
	{"SBC A, H", 1, nullptr, 1},
	{"SBC A, L", 1, nullptr, 1},
	{"SBC A, (HL)", 1, nullptr, 1},
	{"SBC A, A", 1, nullptr, 1},

	{"AND B", 1, nullptr, 1}, /* 0xA0 */
	{"AND C", 1, nullptr, 1},
	{"AND D", 1, nullptr, 1},
	{"AND E", 1, nullptr, 1},
	{"AND H", 1, nullptr, 1},
	{"AND L", 1, nullptr, 1},
	{"AND (HL)", 1, nullptr, 1},
	{"AND A", 1, nullptr, 1},
	{"XOR B", 1, nullptr, 1},
	{"XOR C", 1, nullptr, 1},
	{"XOR D", 1, nullptr, 1},
	{"XOR E", 1, nullptr, 1},
	{"XOR H", 1, nullptr, 1},
	{"XOR L", 1, nullptr, 1},
	{"XOR (HL)", 1, nullptr, 1},
	{"XOR A", 1, nullptr, 1},

	{"OR B", 1, nullptr, 1}, /* 0xB0 */
	{"OR C", 1, nullptr, 1},
	{"OR D", 1, nullptr, 1},
	{"OR E", 1, nullptr, 1},
	{"OR H", 1, nullptr, 1},
	{"OR L", 1, nullptr, 1},
	{"OR (HL)", 1, nullptr, 1},
	{"OR A", 1, nullptr, 1},
	{"CP B", 1, nullptr, 1},
	{"CP C", 1, nullptr, 1},
	{"CP D", 1, nullptr, 1},
	{"CP E", 1, nullptr, 1},
	{"CP H", 1, nullptr, 1},
	{"CP L", 1, nullptr, 1},
	{"CP (HL)", 1, nullptr, 1},
	{"CP A", 1, nullptr, 1},

	{"RET NZ", 1, nullptr, 1}, /* 0xC0 */
	{"POP BC", 1, nullptr, 1},
	{"JP NZ, a16", 3, nullptr, 1},
	{"JP a16", 3, nullptr, 1},
	{"CALL NZ, a16", 3, nullptr, 1},
	{"PUSH BC", 1, nullptr, 1},
	{"ADD  A,d8", 2, nullptr, 1},
	{"RST 00H", 1, nullptr, 1},
	{"RET Z", 1, nullptr, 1},
	{"RET", 1, nullptr, 1},
	{"JP Z, a16", 3, nullptr, 1},
	{"PREFIX CB", 1, nullptr, 1},
	{"CALL Z, a16", 3, nullptr, 1},
	{"CALL a16", 3, nullptr, 1},
	{"ADC  A, d8", 2, nullptr, 1},
	{"RST 08H", 1, nullptr, 1},

	{"RET NC", 1, nullptr, 1}, /* 0xD0 */
	{"POP DE", 1, nullptr, 1},
	{"JP NC, a16", 3, nullptr, 1},
	{"NOT IMPLEMENTED", 1, nullptr, 1},
	{"CALL NC, a16", 3, nullptr, 1},
	{"PUSH DE", 1, nullptr, 1},
	{"SUB  d8", 1, nullptr, 1},
	{"RST 10H", 1, nullptr, 1},
	{"RET C", 1, nullptr, 1},
	{"RETI", 1, nullptr, 1},
	{"JP C, a16", 3, nullptr, 1},
	{"NOT IMPLEMENTED", 1, nullptr, 1},
	{"CALL C, a16", 3, nullptr, 1},
	{"NOT IMPLEMENTED", 1, nullptr, 1},
	{"SBC  A, d8", 2, nullptr, 1},
	{"RST 18H", 1, nullptr, 1},

	{"LDH a8, A", 2, nullptr, 1}, /* 0xE0 */
	{"POP HL", 1, nullptr, 1},
	{"LD (C), A", 1, nullptr, 1},
	{"NOT IMPLEMENTED", 1, nullptr, 1},
	{"NOT IMPLEMENTED", 1, nullptr, 1},
	{"PUSH HL", 1, nullptr, 1},
	{"AND  d8", 2, nullptr, 1},
	{"RST 20H", 1, nullptr, 1},
	{"ADD SP, r8", 2, nullptr, 1},
	{"JP (HL)", 1, nullptr, 1},
	{"LD (a16), A", 3, nullptr, 1},
	{"NOT IMPLEMENTED", 1, nullptr, 1},
	{"NOT IMPLEMENTED", 1, nullptr, 1},
	{"NOT IMPLEMENTED", 1, nullptr, 1},
	{"XOR  d8", 2, nullptr, 1},
	{"RST 28H", 1, nullptr, 1},

	{"LDH a8, A", 2, nullptr, 1}, /* 0xF0 */
	{"POP AF", 1, nullptr, 1},
	{"LD A, (C)", 1, nullptr, 1},
	{"DI", 1, nullptr, 1},
	{"NOT IMPLEMENTED", 1, nullptr, 1},
	{"PUSH AF", 1, nullptr, 1},
	{"OR  d8", 2, nullptr, 1},
	{"RST 30H", 1, nullptr, 1},
	{"LD HL, SP+r8", 2, nullptr, 1},
	{"LD SP, HL", 1, nullptr, 1},
	{"LD A, (a16)", 3, nullptr, 1},
	{"EI", 1, nullptr, 1},
	{"NOT IMPLEMENTED", 1, nullptr, 1},
	{"NOT IMPLEMENTED", 1, nullptr, 1},
	{"CP  d8", 2, nullptr, 1},
	{"RST 38H", 1, nullptr, 1},
};
