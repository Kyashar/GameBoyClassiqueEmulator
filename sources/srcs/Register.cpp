/*
** EPITECH PROJECT, 2019
** Epitech scolarship project (4 years remaining)
** File description:
**      Made on 2019/02 by ebernard
*/

#include "Register.hpp"

std::ostream &operator<<(std::ostream &os, emulator::Register &reg)
{
	os << "a: " << reg.a << "f: " << reg.f << std::endl;
	os << "b: " << reg.b << "c: " << reg.b << std::endl;
	os << "d: " << reg.d << "e: " << reg.e << std::endl;
	os << "h: " << reg.h << "l: " << reg.l << std::endl;
	return os;
}
