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
	std::string fileName(argv[1]);
	/**
	 * read rom
	 */
	rom::Reader rom(fileName);
	rom.readHeader();

	/**
	 * Show nintendo Logo
	 */
	emulator::Cpu cpu(rom.getFile());
	while (cpu.gotSomethingToRead())
		cpu.readInstruction();
	return 0;
}
