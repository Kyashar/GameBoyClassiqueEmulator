/*
** EPITECH PROJECT, 2019
** Epitech scolarship project (4 years remaining)
** File description:
**      Made on 2019/01 by ebernard
*/

#include "Screen.hpp"

gfx::Screen::Screen() :
	_line (0), _clock(0), _mode(0), _beginDisplay(0, 0), _screen()
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
void gfx::Screen::put(size_t timer)
{
	_clock +=  timer;

	if (_mode == 0)
		Hblank();
	else if (_mode == 1)
		Vblank();
	else if (_mode == 2)
		ObjectRead();
	else if (_mode == 3)
		render();
}

void gfx::Screen::Hblank()
{
	if (_clock >= 204)
		return ;
	_line++;
	_clock = 0;

	if (_line == 143) {
		_mode = 1;
		// Render on window;
		_window->clear(sf::Color::Red);
		_window->draw(_pixels.data(), _pixels.size(), sf::Points);
		_window->display();
	} else {
		_mode = 2;
	}
}

void gfx::Screen::Vblank()
{
	if (_clock >= 456)
		return ;
	_mode = 0;
	_line++;
	if (_line > 153) {
		_mode = 2;
		_line = 0;
	}
}

void gfx::Screen::ObjectRead()
{
	if (_clock >= 80)
		return ;
	_clock = 0;
	_mode = 3;
}

void gfx::Screen::render()
{
	if (_clock >= 172)
		return ;
	_clock = 0;
	_mode = 0;

	// write a scanLine to the buffer
}
