/*
** EPITECH PROJECT, 2019
** Epitech scolarship project (4 years remaining)
** File description:
**      Made on 2019/01 by ebernard
*/

#ifndef EMULATOR_GAMEBOY_SCREEN_HPP
#define EMULATOR_GAMEBOY_SCREEN_HPP

#include <iostream>
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
			return {_pixels, height};
		}
		void initWindow();
		void put(size_t timer);
	private:
		void resetScreen();
		void Hblank();
		void Vblank();
		void ObjectRead();
		void renderLine();
		int getAddressFromTile(int tileNB);

//		void setSpriteSet();
		sf::Color getColorFromAddress(int value, unsigned char bit);

		std::unique_ptr<sf::RenderWindow> _window;

		emulator::Memory &_memory;
		std::array<sf::Vertex, 23040> _pixels;

		bool _bgMap;
		bool _bgTile;

		size_t _line;
		size_t _clock;
		char _mode;
		/**
		 *  @location: SCROLLX, SCROOLY register
		 */
		sf::Vector2u _beginDisplay;
	};
}


#endif //EMULATOR_GAMEBOY_SCREEN_HPP
