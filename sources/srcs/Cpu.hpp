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
#include "Memory.hpp"
#include "Register.hpp"
#include "gfx/Screen.hpp"

namespace emulator
{
	class Cpu
	{
	public:
		struct instructionInfos
		{
			std::string _name;
			unsigned char _length;
			void (Cpu::*_instruction)(uint16_t param);
			unsigned char _timer;
		};

		explicit Cpu(std::vector<uint8_t> &instuctions);
		~Cpu();

		bool gotSomethingToRead() const;
		/**
		 * Read instruction and call the right function
		 */
		void readInstruction();

		void Nop(uint16_t) {} /* 0x00 */
		void Ld_BC(uint16_t arg) {_register.bc = arg;}
		void Ld_BC_A(uint16_t) {_memory[_register.pc + _register.bc] = _register.a;}
		void Inc_BC(uint16_t) {_register.bc++;}
		void Inc_B(uint16_t) {_register.b++;}
		void Dec_B(uint16_t) {_register.b--;};
		void Ld_B(uint16_t) {_register.b = _memory[_register.pc];};
		void Rlca(uint16_t) {
			auto ci = _register.a & 0b10000000 ? 1 : 0;

			_register.a = (_register.a << 1) + ci;
			_register.setFlagsC(ci);
		};
		void Ld_SP(uint16_t arg) {_memory[_register.pc + arg] = _register.sp;}
		void Add_HL_BC(uint16_t) {_register.hl += _register.bc;}
		void Ld_A_BC(uint16_t) {_register.a = _memory[_register.pc + _register.bc];}
		void Dec_BC(uint16_t) {_register.bc--;}
		void Inc_C(uint16_t) {_register.c++;}
		void Dec_C(uint16_t) {_register.c--;}
		void Ld_C(uint16_t arg) {_register.c = arg;}
		void Rrca(uint16_t) {
			auto ci = _register.a & 0b00000001 ? 0b10000000 : 0;
			auto co = _register.a & 0b00000001;

			_register.a = (_register.a >> 1) + ci;
			_register.setFlagsC(co);
		}

		void Stop(uint16_t arg) { std::cout << "STOP: " << arg << std::endl; std::cout << "enter slow mode, what to do ?" << arg << std::endl;
		} /*0x10 */

		void default_operator(uint16_t) {std::cout << "unknown operand: " << managedInstruction[_memory[_register.pc]]._name << std::endl; }

	private:
		static std::vector<instructionInfos> managedInstruction;

		Register _register;
		Clock _timer;
		Memory _memory;
		gfx::Screen _gpu;

		size_t _romSize;
		bool _read;
	};
}

std::ostream &operator<<(std::ostream &, const emulator::Cpu::instructionInfos &);

#endif //EMULATOR_GAMEBOY_CPU_HPP
