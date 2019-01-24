/*
** File description:
** description
*/

#include <iostream>
#include "read_rom/Reader.hpp"
#include "Cpu.hpp"

int main(int argc, char **argv)
{
	if (argc != 2) {
		std::cout << "./emulator rom.gba" << std::endl;
		return 1;
	}
	/**
	 * read rom
	 */
	rom::Reader rom(argv[1]);
	rom.readHeader();

	/**
	 * Show nintendo Logo
	 */
	emulator::Cpu cpu(rom.getNintendoData());
	while (cpu.gotSomethingToRead())
		cpu.readInstruction();
	return 0;
}
