/*
** EPITECH PROJECT, 2019
** Epitech scolarship project (4 years remaining)
** File description:
**      Made on 2019/01 by ebernard
*/

#include <iostream>
#include "Cpu.hpp"

emulator::Cpu::Cpu(std::vector<unsigned char> &instuctions) :
	_instruction(instuctions), _read(true)
{
	_register.pc = 0;
	_register.sp = 0;
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
	std::cout << (int)_instruction[_register.pc] << std::endl;
	std::cout << managedInstruction[_instruction[_register.pc]] << std::endl;
	_register.pc += managedInstruction[_instruction[_register.pc]]._length;

	if (_register.pc > 50)
		exit(1);
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
	{"NOP", 1, &Cpu::Nop}, /* 0x00 */
	{"LD BC, d16", 3, nullptr},
	{"LD (BC), A", 1, nullptr},
	{"INC BC", 1, nullptr},
	{"INC B", 1, nullptr},
	{"DEC B", 1, nullptr},
	{"LD B, d8", 2, nullptr},
	{"RLCA", 1, nullptr},
	{"LD (a16), SP", 3, nullptr},
	{"ADD HL, BC", 1, nullptr},
	{"LD A, (BC)", 1, nullptr},
	{"DEC BC", 1, nullptr},
	{"INC C", 1, nullptr},
	{"DEC C", 1, nullptr},
	{"LD C, d8", 2, nullptr},
	{"RRCA", 1, nullptr},

	{"STOP 0", 1, nullptr}, /* 0x10 */
	{"LD DE, d16", 3, nullptr},
	{"LD (DE), A", 1, nullptr},
	{"INC DE", 1, nullptr},
	{"INC D", 1, nullptr},
	{"DEC D", 1, nullptr},
	{"LD D, d8", 2, nullptr},
	{"RLA", 1, nullptr},
	{"JR r8", 2, nullptr},
	{"ADD HL, DE", 1, nullptr},
	{"LD A, (DE)", 1, nullptr},
	{"DEC DE", 1, nullptr},
	{"INC E", 1, nullptr},
	{"DEC E", 1, nullptr},
	{"LD E, d8", 2, nullptr},
	{"RRA", 1, nullptr},

	{"JR NZ, r8", 2, nullptr}, /* 0x20 */
	{"LD HL, d16", 3, nullptr},
	{"LD (HL+), A", 1, nullptr},
	{"INC HL", 1, nullptr},
	{"INC H", 1, nullptr},
	{"DEC H", 1, nullptr},
	{"LD H, d8", 2, nullptr},
	{"DAA", 1, nullptr},
	{"JR Z, r8", 2, nullptr},
	{"ADD HL, HL", 1, nullptr},
	{"LD A, (HL+)", 1, nullptr},
	{"DEC HL", 1, nullptr},
	{"INC L", 1, nullptr},
	{"DEC L", 1, nullptr},
	{"LD L, d8", 2, nullptr},
	{"CPL", 1, nullptr},

	{"JR NC, r8", 2, nullptr}, /* 0x30 */
	{"LD SP, d16", 3, nullptr},
	{"LD (HL-), A", 1, nullptr},
	{"INC SP", 1, nullptr},
	{"INC (HL)", 1, nullptr},
	{"DEC (HL)", 1, nullptr},
	{"LD (HL), d8", 2, nullptr},
	{"SCF", 1, nullptr},
	{"JR C, r8", 2, nullptr},
	{"ADD HL, SP", 1, nullptr},
	{"LD A, (HL-)", 1, nullptr},
	{"DEC SP", 1, nullptr},
	{"INC A", 1, nullptr},
	{"DEC A", 1, nullptr},
	{"LD A, d8", 2, nullptr},
	{"CCF", 1, nullptr},

	{"LD B, B", 1, nullptr}, /* 0x40 */
	{"LD B, C", 1, nullptr},
	{"LD B, D", 1, nullptr},
	{"LD B, E", 1, nullptr},
	{"LD B, H", 1, nullptr},
	{"LD B, L", 1, nullptr},
	{"LD B, (HL)", 1, nullptr},
	{"LD B, A", 1, nullptr},
	{"LD C, B", 1, nullptr},
	{"LD C, C", 1, nullptr},
	{"LD C, D", 1, nullptr},
	{"LD C, E", 1, nullptr},
	{"LD C, H", 1, nullptr},
	{"LD C, L", 1, nullptr},
	{"LD C, (HL)", 1, nullptr},
	{"LD C, A", 1, nullptr},

	{"LD D, B", 1, nullptr}, /* 0x50 */
	{"LD D, C", 1, nullptr},
	{"LD D, D", 1, nullptr},
	{"LD D, E", 1, nullptr},
	{"LD D, H", 1, nullptr},
	{"LD D, L", 1, nullptr},
	{"LD D, (HL)", 1, nullptr},
	{"LD D, A", 1, nullptr},
	{"LD E, B", 1, nullptr},
	{"LD E, C", 1, nullptr},
	{"LD E, D", 1, nullptr},
	{"LD E, E", 1, nullptr},
	{"LD E, H", 1, nullptr},
	{"LD E, L", 1, nullptr},
	{"LD E, (HL)", 1, nullptr},
	{"LD E, A", 1, nullptr},

	{"LD H, B", 1, nullptr}, /* 0x60 */
	{"LD H, C", 1, nullptr},
	{"LD H, D", 1, nullptr},
	{"LD H, E", 1, nullptr},
	{"LD H, H", 1, nullptr},
	{"LD H, L", 1, nullptr},
	{"LD H, (HL)", 1, nullptr},
	{"LD H, A", 1, nullptr},
	{"LD L, B", 1, nullptr},
	{"LD L, C", 1, nullptr},
	{"LD L, D", 1, nullptr},
	{"LD L, E", 1, nullptr},
	{"LD L, H", 1, nullptr},
	{"LD L, L", 1, nullptr},
	{"LD L, (HL)", 1, nullptr},
	{"LD L, A", 1, nullptr},

	{"LD (HL), B", 1, nullptr}, /* 0x70 */
	{"LD (HL), C", 1, nullptr},
	{"LD (HL), D", 1, nullptr},
	{"LD (HL), E", 1, nullptr},
	{"LD (HL), H", 1, nullptr},
	{"LD (HL), L", 1, nullptr},
	{"HALT", 1, nullptr},
	{"LD (HL), A", 1, nullptr},
	{"LD A, B", 1, nullptr},
	{"LD A, C", 1, nullptr},
	{"LD A, D", 1, nullptr},
	{"LD A, E", 1, nullptr},
	{"LD A, H", 1, nullptr},
	{"LD A, L", 1, nullptr},
	{"LD A, (HL)", 1, nullptr},
	{"LD A, A", 1, nullptr},

	{"ADD A, B", 1, nullptr}, /* 0x80 */
	{"ADD A, C", 1, nullptr},
	{"ADD A, D", 1, nullptr},
	{"ADD A, E", 1, nullptr},
	{"ADD A, H", 1, nullptr},
	{"ADD A, L", 1, nullptr},
	{"ADD A, (HL)", 1, nullptr},
	{"ADD A, A", 1, nullptr},
	{"ADC A, B", 1, nullptr},
	{"ADC A, C", 1, nullptr},
	{"ADC A, D", 1, nullptr},
	{"ADC A, E", 1, nullptr},
	{"ADC A, H", 1, nullptr},
	{"ADC A, L", 1, nullptr},
	{"ADC A, (HL)", 1, nullptr},
	{"ADC A, A", 1, nullptr},

	{"SUB B", 1, nullptr}, /* 0x90 */
	{"SUB C", 1, nullptr},
	{"SUB D", 1, nullptr},
	{"SUB E", 1, nullptr},
	{"SUB H", 1, nullptr},
	{"SUB L", 1, nullptr},
	{"SUB (HL)", 1, nullptr},
	{"SUB A", 1, nullptr},
	{"SBC A, B", 1, nullptr},
	{"SBC A, C", 1, nullptr},
	{"SBC A, D", 1, nullptr},
	{"SBC A, E", 1, nullptr},
	{"SBC A, H", 1, nullptr},
	{"SBC A, L", 1, nullptr},
	{"SBC A, (HL)", 1, nullptr},
	{"SBC A, A", 1, nullptr},

	{"AND B", 1, nullptr}, /* 0xA0 */
	{"AND C", 1, nullptr},
	{"AND D", 1, nullptr},
	{"AND E", 1, nullptr},
	{"AND H", 1, nullptr},
	{"AND L", 1, nullptr},
	{"AND (HL)", 1, nullptr},
	{"AND A", 1, nullptr},
	{"XOR B", 1, nullptr},
	{"XOR C", 1, nullptr},
	{"XOR D", 1, nullptr},
	{"XOR E", 1, nullptr},
	{"XOR H", 1, nullptr},
	{"XOR L", 1, nullptr},
	{"XOR (HL)", 1, nullptr},
	{"XOR A", 1, nullptr},

	{"OR B", 1, nullptr}, /* 0xB0 */
	{"OR C", 1, nullptr},
	{"OR D", 1, nullptr},
	{"OR E", 1, nullptr},
	{"OR H", 1, nullptr},
	{"OR L", 1, nullptr},
	{"OR (HL)", 1, nullptr},
	{"OR A", 1, nullptr},
	{"CP B", 1, nullptr},
	{"CP C", 1, nullptr},
	{"CP D", 1, nullptr},
	{"CP E", 1, nullptr},
	{"CP H", 1, nullptr},
	{"CP L", 1, nullptr},
	{"CP (HL)", 1, nullptr},
	{"CP A", 1, nullptr},

	{"RET NZ", 1, nullptr}, /* 0xC0 */
	{"POP BC", 1, nullptr},
	{"JP NZ, a16", 3, nullptr},
	{"JP a16", 3, nullptr},
	{"CALL NZ, a16", 3, nullptr},
	{"PUSH BC", 1, nullptr},
	{"ADD  A,d8", 2, nullptr},
	{"RST 00H", 1, nullptr},
	{"RET Z", 1, nullptr},
	{"RET", 1, nullptr},
	{"JP Z, a16", 3, nullptr},
	{"PREFIX CB", 1, nullptr},
	{"CALL Z, a16", 3, nullptr},
	{"CALL a16", 3, nullptr},
	{"ADC  A, d8", 2, nullptr},
	{"RST 08H", 2, nullptr},

	{"RET NC", 1, nullptr}, /* 0xD0 */
	{"POP DE", 1, nullptr},
	{"JP NC, a16", 3, nullptr},
	{"NOT IMPLEMENTED", 1, nullptr},
	{"CALL NC, a16", 3, nullptr},
	{"PUSH DE", 1, nullptr},
	{"SUB  d8", 1, nullptr},
	{"RST 10H", 1, nullptr},
	{"RET C", 1, nullptr},
	{"RETI", 1, nullptr},
	{"JP C, a16", 3, nullptr},
	{"NOT IMPLEMENTED", 1, nullptr},
	{"CALL C, a16", 3, nullptr},
	{"NOT IMPLEMENTED", 1, nullptr},
	{"SBC  A, d8", 2, nullptr},
	{"RST 18H", 2, nullptr},

	{"LDH a8, A", 2, nullptr}, /* 0xE0 */
	{"POP HL", 1, nullptr},
	{"LD (C), A", 1, nullptr},
	{"NOT IMPLEMENTED", 1, nullptr},
	{"NOT IMPLEMENTED", 1, nullptr},
	{"PUSH HL", 1, nullptr},
	{"AND  d8", 2, nullptr},
	{"RST 20H", 1, nullptr},
	{"ADD SP, r8", 2, nullptr},
	{"JP (HL)", 1, nullptr},
	{"LD (a16), A", 3, nullptr},
	{"NOT IMPLEMENTED", 1, nullptr},
	{"NOT IMPLEMENTED", 1, nullptr},
	{"NOT IMPLEMENTED", 1, nullptr},
	{"XOR  d8", 2, nullptr},
	{"RST 28H", 1, nullptr},

	{"LDH a8, A", 2, nullptr}, /* 0xF0 */
	{"POP AF", 1, nullptr},
	{"LD A, (C)", 1, nullptr},
	{"DI", 1, nullptr},
	{"NOT IMPLEMENTED", 1, nullptr},
	{"PUSH AF", 1, nullptr},
	{"OR  d8", 2, nullptr},
	{"RST 30H", 1, nullptr},
	{"LD HL, SP+r8", 2, nullptr},
	{"LD SP, HL", 1, nullptr},
	{"LD A, (a16)", 3, nullptr},
	{"EI", 1, nullptr},
	{"NOT IMPLEMENTED", 1, nullptr},
	{"NOT IMPLEMENTED", 1, nullptr},
	{"CP  d8", 2, nullptr},
	{"RST 38H", 1, nullptr},
};
