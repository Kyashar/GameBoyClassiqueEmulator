/*
** EPITECH PROJECT, 2019
** Epitech scolarship project (4 years remaining)
** File description:
**      Made on 2019/01 by ebernard
*/

#ifndef EMULATOR_GAMEBOY_REGISTER_HPP
#define EMULATOR_GAMEBOY_REGISTER_HPP

#include <ostream>

namespace emulator
{
	struct Register {
		struct {
			union {
				struct {
					unsigned char f;
					unsigned char a;
				};
				unsigned short af;
			};
		};

		struct {
			union {
				struct {
					unsigned char c;
					unsigned char b;
				};
				unsigned short bc;
			};
		};

		struct {
			union {
				struct {
					unsigned char e;
					unsigned char d;
				};
				unsigned short de;
			};
		};

		struct {
			union {
				struct {
					unsigned char l;
					unsigned char h;
				};
				unsigned short hl;
			};
		};

		struct {
			union {
				struct {
					unsigned char m;
					unsigned char t;
				};
				unsigned short mt;
			};
		};
		unsigned short sp;
		unsigned short pc;

		void setFlagZ(bool b) {
			f = (f & 0b01111111) + (b ? 0b10000000 : 0);
		}
		void setFlagN(bool b) {
			f = (f & 0b10111111) + (b ? 0b01000000 : 0);
		}
		void setFlagH(bool b) {
			f = (f & 0b11011111) + (b ? 0b00100000 : 0);
		}
		void setFlagC(bool b) {
			f = (f & 0b11101111) + (b ? 0b00010000 : 0);
		}
		bool getFlagC() {
			return (f & 0b00010000) != 0;
		}
		bool getFlagH() {
			return (f & 0b00100000) != 0;
		}
		bool getFlagN() {
			return (f & 0b01000000) != 0;
		}
		bool getFlagZ() {
			return (f & 0b10000000) != 0;
		}
	};
	struct Clock {
		unsigned char m;
		unsigned char t;
	};
}

std::ostream &operator<<(std::ostream &os, emulator::Register &reg);

#endif //EMULATOR_GAMEBOY_REGISTER_HPP
