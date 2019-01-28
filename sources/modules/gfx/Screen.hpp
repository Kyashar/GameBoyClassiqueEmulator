/*
** EPITECH PROJECT, 2019
** Epitech scolarship project (4 years remaining)
** File description:
**      Made on 2019/01 by ebernard
*/

#ifndef EMULATOR_GAMEBOY_SCREEN_HPP
#define EMULATOR_GAMEBOY_SCREEN_HPP

#include <SFML/Graphics/RenderWindow.hpp>

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
		Screen();
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
		void render();

		sf::Color getColorFromValue(int value);

		std::array<sf::Vertex, 23040> _pixels;
		std::unique_ptr<sf::RenderWindow> _window;

		size_t _line;
		size_t _clock;
		char _mode;
		/**
		 *  @location: SCROLLX, SCROOLY register
		 */
		sf::Vector2u _beginDisplay;
		/**
		 * 32x32 tiles(8x8 pixels each)
		 * That give a 256x256 screen
		 * But only 160x144 pixel can be display
		 * (SCROLLX, SCROLLY register hold up left corner to begin display)
		 */
		std::array<std::array<unsigned char, 32>, 32> _screen;

	};
}


#endif //EMULATOR_GAMEBOY_SCREEN_HPP
