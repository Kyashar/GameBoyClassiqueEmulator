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
	public:
		Screen() = default;
		~Screen() = default;

		void initWindow();
		void addSprite(sf::Vector2u &pos);
	private:
		std::unique_ptr<sf::RenderWindow> _window;
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
