/*
** EPITECH PROJECT, 2019
** Epitech scolarship project (4 years remaining)
** File description:
**      Made on 2019/01 by ebernard
*/

#include <iomanip>
#include <iostream>
#include <bitset>
#include <SFML/Window/Keyboard.hpp>
#include "Memory.hpp"

emulator::Memory::Memory()  : _biosReaded(false),
			      _bios{0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E,
				    0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0,
				    0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,
				    0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9,
				    0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20,
				    0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04,
				    0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2,
				    0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06,
				    0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xF2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20,
				    0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17,
				    0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
				    0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
				    0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
				    0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3c, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x4C,
				    0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20,
				    0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50},
			      _rom(), _vram(), _eram(), _wram(), _oam(), _key(0b00111111), _zram()
{

}


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
	operator[](index) =     (value & 0b00000000011111111);
	operator[](index + 1) = (value & 0b11111111100000000) >> 8;
}

void emulator::Memory::dumpMemory(int begin, int end)
{
	for (int index = begin; index <= end;) {
		if (index % 16 == 0)
			std::cout << std::setfill('0') << std::setw(8) << std::hex << (int)index << ":";
		if (index % 2 == 0)
			std::cout << std::setw(0) << " ";
		std::cout << std::setfill('0') << std::setw(2) << std::hex << (int)operator[](index);
		index++;
		if (index % 16 == 0 && index != end)
			std::cout << std::setw(0) << std::endl;
	}
	std::cout << std::endl;
}

uint8_t &emulator::Memory::operator[](int addr)
{
	zero = 0;
	if (addr > 0xFFFF || addr < 0)
		throw std::runtime_error("not mapped address");


	if (addr <= 0x0100) {        // BIOS
		if (!_biosReaded) {
			if (addr < 0x0100) {
				return _bios[addr];
			} else {
				std::cout << "end bios" << std::endl;
				_biosReaded = true;
			}
		}
//		dumpMemory(0x9800, 0x9FFF);
		return _rom[addr];
	}
	else if (addr < 0x8000)        // ROM
		return _rom[addr];
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
	else if (addr == 0xFF00) {        // IO register
		updateKeyPressed(_key);
		return _key;
	}
	else if (addr < 0xFF10)
		return zero; // timer
	else if (addr == 0xFF0F)
		return _getInterrupt;
	else if (addr < 0xFF27)
		return zero; // sound system
	else if (addr < 0xFF40)
		return zero;
	else if (addr < 0xFF4C)
		return _registerGpu[addr];
	else if (addr < 0xFF80)
		return zero;
	else if (addr < 0xFFFF)		// zero page
		return _oam[((unsigned int)addr) & ((unsigned int)0x7F)];
	else if (addr == 0xFFFF)
		return _interrupt;
	return zero;
}

void updateKeyPressed(uint8_t &key)
{
	key = (key & 0b00110000) + 0b00001111;

	if ((key & 0b00010000) >> 4) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
			key &= 0b00010111;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
			key &= 0b00011011;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			key &= 0b00011101;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
			key &= 0b00011110;
	} else if ((key & 0b00100000) >> 5) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::O))
			key &= 0b00100111;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
			key &= 0b00101011;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K))
			key &= 0b00101101;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L))
			key &= 0b00101110;
	}
}

std::ostream &operator<<(std::ostream &os, emulator::Memory::GpuRegister &reg)
{
	os << "begin display X: " << (int)reg.bgBeginDisplay.x << std::endl;
	os << "begin display Y: " << (int)reg.bgBeginDisplay.y << std::endl;
	os << "line " << (int)reg.line << std::endl;
	os << "bg palette " << std::bitset<8>(reg.getPalette()) << std::endl;
	os << "bg palette " << (int)reg.getPalette() << std::endl;
	os << "bg control " << std::bitset<8>(reg.getControl()) << std::endl;
	os << "bg control " << (int)reg.getControl() << std::endl;
	os << "bg status " << std::bitset<8>(reg.getStatus()) << std::endl;
	os << "bg status " << (int)reg.getStatus() << std::endl;
	for (int i = 0; i < 4; i++) {
		std::cout << "Color " << i << ": " << (int)reg.getPalette(i) << std::endl;
	}
	return os;
}

