/*
** EPITECH PROJECT, 2019
** Epitech scolarship project (4 years remaining)
** File description:
**      Made on 2019/01 by ebernard
*/

#ifndef EMULATOR_GAMEBOY_READER_HPP
#define EMULATOR_GAMEBOY_READER_HPP

#include <vector>
#include <fstream>
#include <string>
#include <ostream>

namespace rom
{
	class Reader
	{
	public:
		struct RomInfos {
			std::uint32_t EntryPoint;
			char NintendLogo[48];
			char Title[16];
			std::uint16_t NewLicensee;
			std::uint8_t SGB;
			std::uint8_t Cartridge;
			std::uint8_t ROMsize;
			std::uint8_t RAMSize;
			std::uint8_t Country;
			std::uint8_t OldLicensee;
			std::uint8_t Version;
			std::uint8_t HeaderChecksum;
			std::uint16_t GlobalChecksum;
		};

		explicit Reader(std::string fileName);
		void readHeader();
		size_t getRomSize();

		std::vector<uint8_t> &getNintendoData() {
			return _nintendoLogo;
		}

	private:
		std::ifstream _file;
		size_t _sizeRom;

		RomInfos _romInfos;
		std::vector<uint8_t> _nintendoLogo;
		std::vector<uint8_t> _fileContent;
	};
}

std::ostream &operator<<(std::ostream &os, const rom::Reader::RomInfos &infos);


#endif //EMULATOR_GAMEBOY_READER_HPP
