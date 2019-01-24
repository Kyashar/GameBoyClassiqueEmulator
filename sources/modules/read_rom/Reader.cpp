/*
** EPITECH PROJECT, 2019
** Epitech scolarship project (4 years remaining)
** File description:
**      Made on 2019/01 by ebernard
*/

#include <cmath>
#include <iostream>
#include "Reader.hpp"

constexpr char hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

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
//	os << std::hex << (int)infos.HeaderChecksum << std::endl;
//	os << std::hex << (int)infos.GlobalChecksum << std::endl;
	return os;
}


rom::Reader::Reader(std::string fileName) :
	_file(fileName)
{
	_nintendoLogo.resize(256);
}

void rom::Reader::readHeader()
{
	_file.read(reinterpret_cast<char *>(_nintendoLogo.data()), _nintendoLogo.size());
	_file.read(reinterpret_cast<char *>(&_romInfos), sizeof(_romInfos));
	std::cout << _romInfos << std::endl;

	std::cout << "size " << std::to_string(getRomSize()) << std::endl;
	_fileContent.resize(getRomSize());
	_file.read(reinterpret_cast<char *>(_fileContent.data()), _fileContent.size());
}

size_t rom::Reader::getRomSize()
{
	if (_romInfos.EntryPoint == 0)
		throw std::runtime_error("header is Empty");
	_sizeRom = 1;
	_sizeRom <<= (_romInfos.ROMsize + 5);
	return  _sizeRom;
}
