/*
** EPITECH PROJECT, 2019
** Epitech scolarship project (4 years remaining)
** File description:
**      Made on 2019/02 by ebernard
*/

#include <iomanip>
#include "Register.hpp"

std::ostream &operator<<(std::ostream &os, emulator::Register &reg)
{
	os << "a:    " << std::setfill('0') << std::setw(2)  << (int)reg.a << std::setfill('0') << std::setw(2)  << " f:  " << (int)reg.f << std::endl;
	os << "b:    " << std::setfill('0') << std::setw(2)  << (int)reg.b << std::setfill('0') << std::setw(2)  << " c:  " << (int)reg.c << std::endl;
	os << "d:    " << std::setfill('0') << std::setw(2)  << (int)reg.d << std::setfill('0') << std::setw(2)  << " e:  " << (int)reg.e << std::endl;
	os << "h:    " << std::setfill('0') << std::setw(2)  << (int)reg.h << std::setfill('0') << std::setw(2)  << " l:  " << (int)reg.l << std::endl;
	os << "sp: " << std::setfill('0') << std::setw(4)  << (int)reg.sp << std::setfill('0') << std::setw(4)  <<  " pc: " << (int)reg.pc << std::endl;
	os << "af: " << std::setfill('0') << std::setw(4)  << (int)reg.af << std::setfill('0') << std::setw(4)  <<  " bc: " << (int)reg.bc << std::endl;
	os << "de: " << std::setfill('0') << std::setw(4)  << (int)reg.de << std::setfill('0') << std::setw(4)  <<  " hl: " << (int)reg.hl << std::endl;
	os << "Flag: C " << reg.getFlagC() << " Z " << reg.getFlagZ() << " N " << reg.getFlagN() << std::endl;
	return os;
}
