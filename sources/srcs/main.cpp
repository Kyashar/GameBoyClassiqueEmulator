/*
** File description:
** description
*/

#include <iostream>
#include "read_rom/Reader.hpp"

int main(int argc, char **argv)
{
	if (argc != 2) {
		std::cout << "./emulator rom.gba" << std::endl;
		return 1;
	}
	rom::Reader rom(argv[1]);
	rom.readHeader();
	return 0;
}
