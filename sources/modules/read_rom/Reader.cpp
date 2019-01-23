/*
** EPITECH PROJECT, 2019
** Epitech scolarship project (4 years remaining)
** File description:
**      Made on 2019/01 by ebernard
*/

#include <iostream>
#include "Reader.hpp"
char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

std::ostream &operator<<(std::ostream &os, const rom::Reader::RomInfos &infos)
{
	std::string string;
	int i{0};

	os << "Entry point:\t" << std::hex << infos.EntryPoint << std::endl;
	os << "Nitendo Logo:\t";
	for (char elem : infos.NintendLogo) {
		string += hex_chars[ ( elem & 0xF0 ) >> 4 ];
		string += hex_chars[ ( elem & 0x0F ) >> 0 ];
		i++;
		if (i != 0 && i != 48 && i % 16 == 0)
			string += "\n\t\t";
		else
			string += " ";
	}
	os << string << std::endl;
	os << "Game Name:\t" << infos.Title << std::endl;
//	os << "manufacture:\t" << std::hex << infos.ManufactureCode << std::endl;
//	os << "CGB:\t\t" << std::hex << (int)infos.CGB << std::endl;
	os << "Licensse:\t" << std::hex << (int)infos.NewLicensee << std::endl;
	os << "SGB:\t\t" << std::hex << (int)infos.SGB << std::endl;
//	os << "" << std::hex << (int)infos.Cartridge << std::endl;
	os << "rom size:\t" << std::hex << (int)infos.ROMsize << std::endl;
	os << "ram size:\t" << std::hex << (int)infos.RAMSize << std::endl;
	os << "Country:\t" << std::hex << (int)infos.Country << std::endl;
//	os << std::hex << (int)infos.OldLicensee << std::endl;
	os << "Version:\t" << std::hex << (int)infos.Version << std::endl;
	os << std::hex << (int)infos.HeaderChecksum << std::endl;
	os << std::hex << (int)infos.GlobalChecksum << std::endl;

	os << std::hex << sizeof(infos)<< std::endl;
	return os;
}


rom::Reader::Reader(std::string fileName) :
	_file(fileName)
{
}

void rom::Reader::readHeader()
{
	char readFirstByte[256];
	RomInfos infos{0};

	_file.read(readFirstByte, 256);
	_file.read(reinterpret_cast<char *>(&infos), sizeof(infos));
	std::cout << infos << std::endl;
}
