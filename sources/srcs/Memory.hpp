/*
** EPITECH PROJECT, 2019
** Epitech scolarship project (4 years remaining)
** File description:
**      Made on 2019/01 by ebernard
*/

#ifndef EMULATOR_GAMEBOY_MEMORY_HPP
#define EMULATOR_GAMEBOY_MEMORY_HPP

#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include <array>

namespace emulator
{
	/**
	 * emulation for gameboy memory
	 *
	 * $FFFF	Interrupt Enable Flag
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
		struct GpuRegister {
			GpuRegister() : gpuControl(0), gpuStatus(2), beginDisplay(0, 0), line(0), bgPalette(0) {}
			GpuRegister(sf::Vector2u &display, unsigned int &l) : gpuControl(0), beginDisplay(display), line(l), bgPalette(0) {}
			uint8_t &operator[](size_t address) {
				zero = 0;
				if (address == 0xFF40)
					return gpuControl;
				if (address == 0xFF41)
					return gpuStatus;
				if (address == 0xFF42)
					return beginDisplay.y;
				if (address == 0xFF43)
					return beginDisplay.x;
				if (address == 0xFF44)
					return line;
				if (address == 0xFF45)
				 	return cmpline;
				if (address == 0xFF47)
					return bgPalette;
				std::cout << "ERROR: " << address << std::endl;
				return zero;
			}
			unsigned char &getStatus() {return gpuStatus;}
			unsigned char &getControl() {return gpuControl;}
			sf::Vector2<unsigned char> &getDisplay() {return beginDisplay;}
			unsigned char &getLine() {return line;}
			unsigned char &getPalette() {return bgPalette;}
			unsigned char getPalette(unsigned int index) {
				unsigned char masque = 0b11;
				return (bgPalette & (masque << (index * 2))) >> (index * 2);
			}

			bool background() {return gpuControl & 0b1;}
			bool sprite() {return gpuControl & 0b100;}
			sf::Vector2<unsigned char> spriteSize() {return gpuControl & 0b100 ? sf::Vector2<unsigned char>(8, 8) : sf::Vector2<unsigned char>(8, 16);}
			bool bgTileMap() {return gpuControl & 0b1000;}
			bool bgTileSet() {return gpuControl & 0b10000;}
			bool window() {return gpuControl & 0b100000;}
			bool windowTileMap() {return gpuControl & 0b1000000;}
			bool display() {return gpuControl & 0b10000000;}

			void setPalette(unsigned int index, unsigned int value) {
				bgPalette &= ~(0b11 << (index * 2));
				bgPalette = (bgPalette | (value << (index * 2)));
			}

			unsigned char gpuControl;
			unsigned char gpuStatus;
			sf::Vector2<unsigned char> beginDisplay;
			unsigned char line;
			unsigned char cmpline;
			unsigned char bgPalette;
			unsigned char zero;
			unsigned char o = 0x90;
		};

		Memory();
		~Memory() = default;

		void loadRom(std::vector<uint8_t> &data);
		uint16_t getShort(int index);
		void setShort(int index, uint16_t value);
		void dumpMemory(int begin, int end);
		std::array<uint8_t, 32768> getRom() {return _rom;}

		GpuRegister  &getGpuRegister() {return _registerGpu;}
		uint8_t &operator[](int index);
	private:
//		std::array<uint8_t , 65536> _memory;

		uint8_t zero = 0;
		bool _biosReaded;
		GpuRegister _registerGpu; // 0xFF40 -> 0xFF48

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

std::ostream &operator<<(std::ostream &os, emulator::Memory::GpuRegister &reg);


#endif //EMULATOR_GAMEBOY_MEMORY_HPP
