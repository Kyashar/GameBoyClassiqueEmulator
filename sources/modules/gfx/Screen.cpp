/*
** EPITECH PROJECT, 2019
** Epitech scolarship project (4 years remaining)
** File description:
**      Made on 2019/01 by ebernard
*/

#include <SFML/Window/Keyboard.hpp>
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

	if (_memory.getGpuRegister().sprite())
		exit(84);
	// Which line of tiles to use in the map
	addr += (((_memory.getGpuRegister().getLine() + _memory.getGpuRegister().getDisplay().y) & 255)/ 8) * 32;
	lineoffs = _memory.getGpuRegister().getDisplay().x / 8;

	// Where to begin in the tile
	x = _memory.getGpuRegister().getDisplay().x % 8;
	y = ((_memory.getGpuRegister().getLine() + _memory.getGpuRegister().getDisplay().y) & 255) % 8;
	bit = (128 >> x);

	tileNumber = _memory[addr + lineoffs];
	tileAdrresse = getAddressFromTile(tileNumber, y);
	for (size_t i = 0; i < 160; i++) {
		operator[](_memory.getGpuRegister().getLine())[i].color = getColorFromAddress(tileAdrresse, bit, x);
		bit >>= 1;
		x++;
		if (x == 8)  {
			x = 0;
			bit = 128;
			lineoffs = (lineoffs + 1) % 32;
			tileNumber = _memory[addr + lineoffs];
			tileAdrresse = getAddressFromTile(tileNumber, y);
		}
		if (bit == 0 || x > 8)
			exit(84);
	}
	renderLineSprite();
}

void gfx::Screen::renderLineSprite()
{
	sf::Vector2i cord;
	uint8_t tileNumber;
	uint8_t options;
	int x;
	int bit;
	int tileAdrresse;

	for (int i = 0; i < 40; i++){
		cord.y = _memory[0xFE00 + i * 4];
		cord.x = _memory[0xFE00 + i * 4 + 1];
		tileNumber = _memory[0xFE00 + i * 4 + 2];
		options =  _memory[0xFE00 + i * 4 + 3];

		if ((cord.y > _memory.getGpuRegister().getLine() || (cord.y + 8) <= _memory.getGpuRegister().getLine()) || !((options & 0b10000000) >> 7))
			continue ;
//		std::cout << "sprite nb: " << std::dec  << i << std::endl;
//		std::cout << "tile nb: " << std::dec << (int)tileNumber << std::endl;
//		std::cout << (int)cord.x << std::endl;
//		std::cout << (int)cord.y << std::endl;
//		std::cout << std::endl;

		tileAdrresse = getAddressFromTile(tileNumber, _memory.getGpuRegister().getLine() - cord.y);
		bit = 128;
		for (size_t i = 0; i < 8; i++) {
			operator[](_memory.getGpuRegister().getLine())[i + cord. x].color = getColorFromAddress(tileAdrresse, bit, i, options);
			bit >>= 1;
		}
	}
}

int gfx::Screen::getAddressFromTile(int tileNb, int line)
{
	if (!_memory.getGpuRegister().bgTileSet())
		tileNb += 256;
	return 0x8000 + (tileNb * 8 * 2) + (line * 2);
}

sf::Color gfx::Screen::getColorFromAddress(int address, unsigned char bit, unsigned  char x, char spriteOptions)
{
	int value = ((_memory[address] & bit) >> (7 - x)) + (((_memory[address + 1] & bit)) >> (7 - x));
	auto palette = (spriteOptions & 0b10000) >> 4;

	if (0 == _memory.getGpuRegister().getPalette(value, palette))
		return sf::Color(255, 255, 255);
	if (1 == _memory.getGpuRegister().getPalette(value, palette))
		return sf::Color(192, 192, 192);
	if (2 == _memory.getGpuRegister().getPalette(value, palette))
		return sf::Color(96, 96, 96);
	return sf::Color(0, 0, 0);
}

sf::Color gfx::Screen::getColorFromAddress(int address, unsigned char bit, unsigned  char x)
{
	int value = ((_memory[address] & bit) >> (7 - x)) + (((_memory[address + 1] & bit)) >> (7 - x));

	if (0 == _memory.getGpuRegister().getPalette(value))
		return sf::Color(255, 255, 255);
	if (1 == _memory.getGpuRegister().getPalette(value))
		return sf::Color(192, 192, 192);
	if (2 == _memory.getGpuRegister().getPalette(value))
		return sf::Color(96, 96, 96);
	return sf::Color(0, 0, 0);
}

#include <bitset>

void gfx::Screen::updateKeyPressed()
{
	_memory[0xFF00] &= 0b00111111;

	if ((_memory[0xFF00] & 0b00010000) >> 4) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) _memory[0xFF00] &= 0b00110111; else _memory[0xFF00] |= 0b00001000;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) _memory[0xFF00] &= 0b00111011; else _memory[0xFF00] |= 0b00000100;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) _memory[0xFF00] &= 0b00111101; else _memory[0xFF00] |= 0b00000010;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) _memory[0xFF00] &= 0b00111110; else _memory[0xFF00] |= 0b00000001;
	} else if ((_memory[0xFF00] & 0b00100000) >> 5){
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) _memory[0xFF00] &= 0b00110111; else _memory[0xFF00] |= 0b00001000;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) _memory[0xFF00] &= 0b00111011; else _memory[0xFF00] |= 0b00000100;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) _memory[0xFF00] &= 0b00111101; else _memory[0xFF00] |= 0b00000010;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) _memory[0xFF00] &= 0b00111110; else _memory[0xFF00] |= 0b00000001;
	}
}

