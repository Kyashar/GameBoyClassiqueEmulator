/*
** EPITECH PROJECT, 2019
** Epitech scolarship project (4 years remaining)
** File description:
**      Made on 2019/01 by ebernard
*/

#include "Screen.hpp"

gfx::Screen::Screen() :
	_beginDisplay(0, 0)
{
}

/**
 * Init sfml window
 */
void gfx::Screen::initWindow()
{
	_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(160, 144, 4), "EmulIt");
	_window->clear(sf::Color::Red);
	_window->draw(_pixels.data(), _pixels.size(), sf::Points);
	_window->display();
}

/**
 * Emulate the 4 step of a gameboy GPU
 * First: Horizontal blank after each line
 * Second: If he is at the bottom vertical blank and put the screen
 * Third: OAM read (blank) until the GPU passe the right time
 * Fourth: Render the image
 */
void gfx::Screen::put()
{

}

void gfx::Screen::Hblank()
{

}

void gfx::Screen::Vblank()
{

}

void gfx::Screen::ObjectRead()
{

}

void gfx::Screen::render()
{

}
