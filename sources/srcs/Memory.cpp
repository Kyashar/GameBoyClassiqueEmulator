/*
** EPITECH PROJECT, 2019
** Epitech scolarship project (4 years remaining)
** File description:
**      Made on 2019/01 by ebernard
*/

#include "Memory.hpp"

void emulator::Memory::loadRom(std::vector<uint8_t> &data)
{
	std::copy(data.begin(), data.end(), _rom.begin());
}

uint16_t emulator::Memory::getShort(int index)
{
	uint16_t first = (*this)[index];
	uint16_t seconde = (*this)[index + 1] << 8;

	uint16_t ret = first + seconde;
	return ret;
}

uint16_t emulator::Memory::getShort(int index) const
{
	uint16_t first = (*this)[index];
	uint16_t seconde = (*this)[index + 1] << 8;

	uint16_t ret = first + seconde;
	return ret;
}

uint8_t &emulator::Memory::operator[](int index)
{
	if (index > static_cast<ssize_t>(_memory.size()) || index < 0)
		throw std::runtime_error("not mapped address");
	return _memory[index];
}

const uint8_t &emulator::Memory::operator[](int index) const
{
	if (index > static_cast<ssize_t>(_memory.size()) || index < 0)
		throw std::runtime_error("not mapped address");
	return _memory[index];
}
