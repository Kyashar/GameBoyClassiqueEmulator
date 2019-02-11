/*
** EPITECH PROJECT, 2019
** Epitech scolarship project (4 years remaining)
** File description:
**      Made on 2019/01 by ebernard
*/

#include <iomanip>
#include <iostream>
#include "Memory.hpp"

void emulator::Memory::loadRom(std::vector<uint8_t> &data)
{
	std::copy(data.begin(), data.end(), _rom.begin());
}

uint16_t emulator::Memory::getShort(int index)
{
	uint16_t first = operator[](index);
	uint16_t seconde = operator[](index + 1) << 8;

	uint16_t ret = first + seconde;
	return ret;
}

void emulator::Memory::setShort(int index, uint16_t value)
{
	operator[](index) =     (value & 0b11111111000000000) >> 8;
	operator[](index + 1) = (value & 0b00000000111111111);
}

uint8_t &emulator::Memory::operator[](int addr)
{
	if (addr > 0xFFFF || addr < 0)
		throw std::runtime_error("not mapped address");

	if (addr < 0x150 && !_biosReaded)	// BIOS
		return _bios[addr];
	else if (addr < 0x8000) {        // ROM
		return _rom[addr];
	}
	else if (addr < 0xA000)		// VRAM
		return _vram[((unsigned int)addr) & ((unsigned int)0x1FFF)];
	else if (addr < 0xC000)		// ERAM
		return _eram[((unsigned int)addr) & ((unsigned int)0x1FFF)];
	else if (addr < 0xE000)		// WRWAM
		return _wram[((unsigned int)addr) & ((unsigned int)0x1FFF)];
	else if (addr < 0xFE00)		//shadwo WRAM
		return _wram[((unsigned int)addr) & ((unsigned int)0x1FFF)];
	else if (addr < 0xFEA0)		// OAM
		return _oam[((unsigned int)addr) & ((unsigned int)0xFF)];
	else if (addr < 0xFF00)		// unsabel memory
		return zero;
	else if (addr < 0xFF40)
		return zero;
	else if (addr < 0xFF4C)
		return _registerGpu[addr];
	else if (addr < 0xFF80)		// IO register
		return zero;
	else if (addr < 0xFFFF)		// zero page
		return _oam[((unsigned int)addr) & ((unsigned int)0x7F)];
	return zero;
}

//const uint8_t &emulator::Memory::operator[](int addr) const
//{
//	if (addr > 0xFFFF || addr < 0)
//		throw std::runtime_error("not mapped address");
//
//	if (addr < 0x150 && !_biosReaded)	// BIOS
//		return _bios[addr];
//	else if (addr < 0x8000) 	// ROM
//		return _rom[addr];
//	else if (addr < 0xA000)		// VRAM
//		return _vram[((unsigned int)addr) & ((unsigned int)0x1FFF)];
//	else if (addr < 0xC000)		// ERAM
//		return _eram[((unsigned int)addr) & ((unsigned int)0x1FFF)];
//	else if (addr < 0xE000)		// WRWAM
//		return _wram[((unsigned int)addr) & ((unsigned int)0x1FFF)];
//	else if (addr < 0xFE00)		//shadwo WRAM
//		return _wram[((unsigned int)addr) & ((unsigned int)0x1FFF)];
//	else if (addr < 0xFEA0)		// OAM
//		return _oam[((unsigned int)addr) & ((unsigned int)0xFF)];
//	else if (addr < 0xFF00)		// unsabel memory
//		return zero;
//	else if (addr < 0xFF4C)
//		return _registerGpu[addr];
//	else if (addr < 0xFF80)		// IO register
//		return zero;
//	else if (addr < 0xFFFF)		// zero page
//		return _oam[((unsigned int)addr) & ((unsigned int)0x7F)];
//	return zero;
//}
