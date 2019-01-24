/*
** EPITECH PROJECT, 2019
** Epitech scolarship project (4 years remaining)
** File description:
**      Made on 2019/01 by ebernard
*/

#ifndef EMULATOR_GAMEBOY_MEMORY_HPP
#define EMULATOR_GAMEBOY_MEMORY_HPP

#include <array>

namespace emulator
{
	class Memory
	{
	public:
		Memory();
		~Memory();

		int &operator[](int index) {
			if (index > _memory.size() || index < 0)
				throw std::runtime_error("not mapped address");
			return _memory[index];
		}

		const int &operator[](int index) const  {
			if (index > _memory.size() || index < 0)
				throw std::runtime_error("not mapped address");
			return _memory[index];
		}
	private:
		std::array<int, 65536> _memory;
	};
}


#endif //EMULATOR_GAMEBOY_MEMORY_HPP
