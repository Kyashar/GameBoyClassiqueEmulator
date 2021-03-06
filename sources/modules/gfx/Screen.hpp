/*
** EPITECH PROJECT, 2019
** Epitech scolarship project (4 years remaining)
** File description:
**      Made on 2019/01 by ebernard
*/

#ifndef EMULATOR_GAMEBOY_SCREEN_HPP
#define EMULATOR_GAMEBOY_SCREEN_HPP

#include <iostream>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../../srcs/Memory.hpp"

namespace gfx {

	class Screen
	{
		struct ScreenLine
		{
		public:
			ScreenLine(std::array<sf::Vertex, 23040> &array, size_t height) :
				_array(array), _height(height) {}
			~ScreenLine() = default;

			sf::Vertex &operator[](size_t width) {
				return _array[_height * 160 + width];
			}
		private:
			std::array<sf::Vertex, 23040> &_array;
			size_t _height;
		};
	public:
		explicit Screen(emulator::Memory &mem);
		~Screen() = default;

		ScreenLine operator[](size_t height) {
			return ScreenLine(_pixels, height);
		}
		void setMemory(emulator::Memory &mem){_memory = mem; _register = mem._registerGpu;}
		void initWindow();
		void put(size_t timer);
		void updateKeyPressed();
	private:
		void resetScreen();
		void Hblank();
		void Vblank();
		void ObjectRead();

		void renderLine();
		void renderLineSprite();
		int getAddressFromTile(int tileNB, int line);

		void crossKeysEvent();

		sf::Color getColorFromAddress(int address, unsigned char bit, unsigned char x);
		sf::Color getColorFromAddress(int address, unsigned char bit, unsigned  char x, char spriteOptions);

		std::unique_ptr<sf::RenderWindow> _window;
		sf::Event _event;

		emulator::Memory &_memory;
		emulator::Memory::GpuRegister &_register;
		std::array<sf::Vertex, 23040> _pixels;

		size_t _vBlank;
		size_t _clock;
		char _mode;
		uint8_t tmp;
	};
}


#endif //EMULATOR_GAMEBOY_SCREEN_HPP
