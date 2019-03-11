/*
** EPITECH PROJECT, 2019
** Epitech scolarship project (4 years remaining)
** File description:
**      Made on 2019/01 by ebernard
*/

#include <bitset>
#include <SFML/Window/Keyboard.hpp>
#include "Screen.hpp"

gfx::Screen::Screen(emulator::Memory &mem) :
	_memory(mem), _register(mem._registerGpu), _vBlank(0), _clock(0), _mode(0), tmp(0b11)
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

	if (_register.gpuInterupt != tmp) {
		_register.gpuStatus = _register.gpuInterupt;
		_register.gpuInterupt = tmp;
	} else if (_register.getInter) {
		_register.getInter = false;
	}


	if (_mode == 0)
		Hblank();
	else if (_mode == 1)
		Vblank();
	else if (_mode == 2)
		ObjectRead();
	else if (_mode == 3) {
		renderLine();
	}

	tmp &= ~(0b11);
	tmp |= _mode;
	_register.gpuInterupt = tmp;
	if (_register.cmpline == _register.line) {
		_register.gpuInterupt |= 0b100;
		_register.gpuInterupt |= 0b1000000;
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
	_register.line++;
	_clock = 0;

	if (_register.line == 143) {

		if (_register.gpuStatus & 2) {
			_register.gpuInterupt |= 0b10000;
			_memory._getInterrupt |= 2;
		}
		_register.line++;
		_mode = 1;
	} else {
		if (_register.gpuStatus & 4) {
			_register.gpuInterupt |= 0b1000000;
			_memory._getInterrupt |= 2;
		}
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
	_clock = 0;
	_vBlank++;
	_register.line++;
//	if (_vBlank > 9) {
	if (_register.line > 153) {
		_mode = 2;
		_register.line = 0;
		_vBlank = 0;
		if (_register.gpuStatus & 0b1000)
			tmp |= 0b1000;
		_window->clear(sf::Color::Red);
		_window->draw(_pixels.data(), _pixels.size(), sf::Points);
		_window->display();
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
	int addr = _register.bgTileMap() ? 0x9C00 : 0x9800;
	int lineoffs;
	unsigned char bit;
	unsigned int x;
	unsigned int y;
	_clock = 0;
	_mode = 0;

	if (_register.gpuStatus & 1) {
		_register.gpuInterupt |= 0b1000;
		_memory._getInterrupt |= 2;
	}
	// Which line of tiles to use in the map
	addr += (((_register.line + _register.getDisplay().y) & 255)/ 8) * 32;
	lineoffs = _register.getDisplay().x / 8;

	// Where to begin in the tile
	x = _register.getDisplay().x % 8;
	y = ((_register.line + _register.getDisplay().y) & 255) % 8;
	bit = (128 >> x);

	tileNumber = _memory[addr + lineoffs];
	tileAdrresse = getAddressFromTile(tileNumber, y);
	for (size_t i = 0; i < 160; i++) {
		operator[](_register.line)[i].color = getColorFromAddress(tileAdrresse, bit, x);
		bit >>= 1;
		x++;
		if (x == 8)  {
			x = 0;
			bit = 128;
			lineoffs = (lineoffs + 1) % 32;
			tileNumber = _memory[addr + lineoffs];
			tileAdrresse = getAddressFromTile(tileNumber, y);
		}
	}
	renderLineSprite();
}

void gfx::Screen::renderLineSprite()
{
	sf::Vector2i cord;
	uint8_t tileNumber;
	uint8_t options;
	int bit;
	int tileAdrresse;

	for (int i = 0; i < 40 && _register.line < 144; i++){
		cord.y = _memory._wram[0xE00 + i * 4];
		cord.x = _memory._wram[0xE00 + i * 4 + 1];
		tileNumber = _memory._wram[0xE00 + i * 4 + 2];
		options =  _memory._wram[0xE00 + i * 4 + 3];

		if ((cord.y > _register.line || (cord.y + 8) <= _register.line) || !((options & 0b10000000) >> 7))
			continue ;

		tileAdrresse = getAddressFromTile(tileNumber, _register.line - cord.y);
		bit = 128;
		for (size_t i = 0; i < 8; i++) {
			operator[](_register.line)[i + cord. x].color = getColorFromAddress(tileAdrresse, bit, i, options);
			bit >>= 1;
		}
	}
}

int gfx::Screen::getAddressFromTile(int tileNb, int line)
{
	if (!_register.bgTileSet())
		tileNb += 256;
	return (tileNb * 8 * 2) + (line * 2);
}

sf::Color gfx::Screen::getColorFromAddress(int address, unsigned char bit, unsigned  char x, char spriteOptions)
{
	int value = ((_memory._vram[address] & bit) >> (7 - x)) + (((_memory._vram[address + 1] & bit)) >> (7 - x));
	auto palette = (spriteOptions & 0b10000) >> 4;

	if (0 == _register.getPalette(value, palette))
		return sf::Color(255, 255, 255);
	if (1 == _register.getPalette(value, palette))
		return sf::Color(192, 192, 192);
	if (2 == _register.getPalette(value, palette))
		return sf::Color(96, 96, 96);
	return sf::Color(0, 0, 0);
}

sf::Color gfx::Screen::getColorFromAddress(int address, unsigned char bit, unsigned  char x)
{
	int value = ((_memory._vram[address] & bit) >> (7 - x)) + (((_memory._vram[address + 1] & bit)) >> (7 - x));

	if (0 == _memory.getGpuRegister().getPalette(value))
		return sf::Color(255, 255, 255);
	if (1 == _memory.getGpuRegister().getPalette(value))
		return sf::Color(192, 192, 192);
	if (2 == _memory.getGpuRegister().getPalette(value))
		return sf::Color(96, 96, 96);
	return sf::Color(0, 0, 0);
}
