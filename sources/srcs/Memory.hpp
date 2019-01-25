/*
** EPITECH PROJECT, 2019
** Epitech scolarship project (4 years remaining)
** File description:
**      Made on 2019/01 by ebernard
*/

#ifndef EMULATOR_GAMEBOY_MEMORY_HPP
#define EMULATOR_GAMEBOY_MEMORY_HPP

#include <vector>
#include <array>

namespace emulator
{
	/**
	 * emulation for gameboy memory
	 *
	 * $FFFF		Interrupt Enable Flag
	 * $FF80-$FFFE	Zero Page - 127 bytes
	 * $FF00-$FF7F	Hardware I/O Registers
	 *
	 * $FEA0-$FEFF	Unusable Memory
	 *
	 * $FE00-$FE9F	OAM - Object Attribute Memory			-> Graphics sprite info
	 *
	 * $E000-$FDFF	Echo RAM - Reserved, Do Not Use
	 * $D000-$DFFF	Internal RAM - Bank 1-7 (switchable - CGB only)
	 * $C000-$CFFF	Internal RAM - Bank 0 (fixed)
	 *
	 * $A000-$BFFF	Cartridge RAM (If Available)
	 *
	 * $9C00-$9FFF	BG Map Data 2 --|
	 * $9800-$9BFF	BG Map Data 1 	|				-> graphics Ram
	 * $8000-$97FF	Character RAM --|
	 *
	 * $4000-$7FFF	Cartridge ROM - Switchable Banks 1-xx
	 * $0150-$3FFF	Cartridge ROM - Bank 0 (fixed)
	 *
	 * $0100-$014F	Cartridge Header Area				-> Data about its name and maufacturer
	 * $0000-$00FF	Restart and Interrupt Vectors 			-> BIOS
	 */
	class Memory
	{
	public:
		Memory() : _biosReaded(true) {}
		~Memory() = default;

		void loadRom(std::vector<uint8_t> &data);
		uint16_t getShort(int index);
		uint16_t getShort(int index)  const;

		uint8_t &operator[](int index);
		const uint8_t &operator[](int index) const;
	private:
		uint8_t zero = 0;
		std::array<uint8_t , 65536> _memory;
		bool _biosReaded;

		/**
		 * each memory is in different array
		 * one array -> pointer to the different address
		 */
		std::array<uint8_t, 256> _bios; 	// go the first instruction to load the gameboy
		std::array<uint8_t, 32768> _rom; 	// rom bank
		std::array<uint8_t, 8192> _vram; 	// graphique ram
		std::array<uint8_t, 8192> _eram;	// external ram

		std::array<uint8_t, 8192> _wram; 	// working ram
		std::array<uint8_t, 160> _oam;
		// memory mapped I/O 		  -> std::array<uint8_t, 128>
		std::array<uint8_t, 128> _zram;
	};
}


#endif //EMULATOR_GAMEBOY_MEMORY_HPP
