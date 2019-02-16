/*
** EPITECH PROJECT, 2019
** Epitech scolarship project (4 years remaining)
** File description:
**      Made on 2019/01 by ebernard
*/

#include "Screen.hpp"

gfx::Screen::Screen(emulator::Memory &mem) :
	_memory(mem), _clock(0), _mode(0)
{
	sf::Vector2f pos = {0, 1};

	for (auto &elem  : _pixels) {
		elem.position = pos;
		elem.color = sf::Color::Black;
		pos.x++;
		if (pos.x == 160) {
			pos.x = 0;
			pos.y++;
		}
	}
}

/**
 * Init sfml window
 */
void gfx::Screen::initWindow()
{
	_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(160, 144), "EmulIt", sf::Style::Close);
	resetScreen();
	_window->display();
}

/**
 * Reset Screen color to black
 */
void gfx::Screen::resetScreen()
{
	for (auto &elem : _pixels){
		elem.color = sf::Color::Black;
	}
	_window->draw(_pixels.data(), _pixels.size(), sf::Points);
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
	_clock += timer;

	if (_mode == 0)
		Hblank();
	else if (_mode == 1)
		Vblank();
	else if (_mode == 2)
		ObjectRead();
	else if (_mode == 3) {
		renderLine();
	}
}

/**
 * emulate gpu timer
 * horizontal blank line after each line can
 */
void gfx::Screen::Hblank()
{
	if (_clock < 204)
		return ;
	auto &line = _memory.getGpuRegister().getLine();
	line++;
	_clock = 0;

	if (line == 143) {
		_mode = 1;
		_window->clear(sf::Color::Red);
		_window->draw(_pixels.data(), _pixels.size(), sf::Points);
		_window->display();
	} else {
		_mode = 2;
	}
}


/**
 * emulate gpu timer for vertical blank line. (10 lines)
 */
void gfx::Screen::Vblank()
{
	if (_clock < 456)
		return ;
	_mode = 1;
	_clock = 0;
	_memory.getGpuRegister().getLine()++;
	if (_memory.getGpuRegister().getLine() > 153) {
		_mode = 2;
		_memory.getGpuRegister().getLine() = 0;
	}
}

void gfx::Screen::ObjectRead()
{
	if (_clock < 80)
		return ;
	_clock = 0;
	_mode = 3;
}

void gfx::Screen::renderLine()
{
	if (_clock < 172)
		return ;

	char tileNumber;
	int tileAdrresse;
	int addr = _memory.getGpuRegister().bgTileMap() ? 0x9C00 : 0x9800;
	int lineoffs;
	unsigned char bit;
	unsigned int x;
	unsigned int y;
	_clock = 0;
	_mode = 0;

	// Which line of tiles to use in the map
	addr += (((_memory.getGpuRegister().getLine() + _memory.getGpuRegister().getDisplay().y) & 255)/ 8) * 32;
	lineoffs = _memory.getGpuRegister().getDisplay().x / 8;

	// get addr from tile for line and index
	x = _memory.getGpuRegister().getDisplay().x % 8;
	y = ((_memory.getGpuRegister().getLine() + _memory.getGpuRegister().getDisplay().y) & 255) % 8;
	bit = (128 >> x);

	tileNumber = _memory[addr + lineoffs];
	tileAdrresse = getAddressFromTile(tileNumber, y);
	for (size_t i = 0; i < 160; i++) {
		operator[](_memory.getGpuRegister().getLine())[i].color = getColorFromAddress(tileAdrresse, bit);
		bit >>= 1;
		x++;
		if (x == 8)  {
			x = 0;
			bit = 128;
			lineoffs = (lineoffs + 1) & 31;
			tileNumber = _memory[addr + lineoffs];
			tileAdrresse = getAddressFromTile(tileNumber, y);
		}
		if (bit == 0 || x > 8)
			exit(84);
	}
}

int gfx::Screen::getAddressFromTile(int tileNb, int line)
{
	if (!_memory.getGpuRegister().bgTileSet())
		tileNb += 256;
	return 0x8000 + (tileNb * 8 * 2) + (line * 2);
}

sf::Color gfx::Screen::getColorFromAddress(int address, unsigned char bit)
{
	int value = (_memory[address] & bit) + (_memory[address + 1] & bit);

	if (0 == _memory.getGpuRegister().getPalette(value))
		return sf::Color(255, 255, 255);
	if (1 == _memory.getGpuRegister().getPalette(value))
		return sf::Color(192, 192, 192);
	if (2 == _memory.getGpuRegister().getPalette(value))
		return sf::Color(96, 96, 96);
	return sf::Color(0, 0, 0);
}
