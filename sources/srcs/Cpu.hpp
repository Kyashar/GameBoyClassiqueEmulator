/*
** EPITECH PROJECT, 2019
** Epitech scolarship project (4 years remaining)
** File description:
**      Made on 2019/01 by ebernard
*/

#ifndef EMULATOR_GAMEBOY_CPU_HPP
#define EMULATOR_GAMEBOY_CPU_HPP

#include <string>
#include <vector>
#include "Register.hpp"

namespace emulator
{
	class Cpu
	{
	public:
		struct instructionInfos
		{
			std::string _name;
			unsigned char _length;
			void (Cpu::*instruction)();
		};

		Cpu(std::vector<unsigned char> &instuctions);
		~Cpu();

		bool gotSomethingToRead() const;
		/**
		 * Read instruction and call the right function
		 */
		 void readInstruction();

		 void Nop();
		 void Ld();

	private:
		static std::vector<instructionInfos> managedInstruction;

		Register _register;
		std::vector<unsigned char> _instruction;
		bool _read;
	};
}

std::ostream &operator<<(std::ostream &, const emulator::Cpu::instructionInfos &);

#endif //EMULATOR_GAMEBOY_CPU_HPP
