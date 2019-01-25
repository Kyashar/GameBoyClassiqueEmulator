/*
** EPITECH PROJECT, 2019
** Epitech scolarship project (4 years remaining)
** File description:
**      Made on 2019/01 by ebernard
*/

#ifndef EMULATOR_GAMEBOY_REGISTER_HPP
#define EMULATOR_GAMEBOY_REGISTER_HPP

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
	};
	struct Clock {
		unsigned char m;
		unsigned char t;
	};
}

#endif //EMULATOR_GAMEBOY_REGISTER_HPP
