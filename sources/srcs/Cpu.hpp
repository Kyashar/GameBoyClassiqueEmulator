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

	private:
		/* 0x00 */
		void Nop(uint16_t) {}
		void Ld_BC(uint16_t arg) {_register.bc = arg;}
		void Ld_BC_A(uint16_t) {_memory[_register.pc + _register.bc] = _register.a;}
		void Inc_BC(uint16_t) {_register.bc++;}
		void Inc_B(uint16_t) {_register.b++;}
		void Dec_B(uint16_t) {_register.b--;};
		void Ld_B(uint16_t) {_register.b = _memory[_register.pc];};
		void Rlca(uint16_t) {
			auto ci = _register.a & 0b10000000 ? 1 : 0;

			_register.a = (_register.a << 1) + ci;
			_register.setFlagC(ci);
		};
		void Ld_SP(uint16_t arg) {_memory[_register.pc + arg] = _register.sp;}
		void Add_HL_BC(uint16_t) {_register.hl += _register.bc; _register.setFlagN(false);}
		void Ld_A_BC(uint16_t) {_register.a = _memory[_register.pc + _register.bc];}
		void Dec_BC(uint16_t) {_register.bc--;}
		void Inc_C(uint16_t) {_register.c++;}
		void Dec_C(uint16_t) {_register.c--;}
		void Ld_C(uint16_t arg) {_register.c = arg;}
		void Rrca(uint16_t) {
			auto ci = _register.a & 0b00000001 ? 0b10000000 : 0;
			auto co = _register.a & 0b00000001;

			_register.a = (_register.a >> 1) + ci;
			_register.setFlagC(co);
		}

		/*0x10 */
		void Stop(uint16_t arg) { std::cout << "STOP: " << arg << std::endl << "enter slow mode, what to do ?" << arg << std::endl;
		}
		void Ld_DE(uint16_t arg) {_register.de = arg;}
		void Ld_DE_A(uint16_t) {_memory[_register.pc + _register.de] = _register.a;}
		void Inc_DE(uint16_t) {_register.de++;}
		void Inc_D(uint16_t) {_register.d++;}
		void Dec_D(uint16_t) {_register.d--;}
		void Ld_D(uint16_t arg) {_register.e = arg;}
		void Rla(uint16_t) {
			auto ci= _register.getFlagC();
			auto co= (_register.a & 0b10000000) != 0;

			_register.a= (_register.a << 1) + ci;
			_register.setFlagC(co);
		}
		void Jr(uint16_t arg) {_register.pc += arg;}
		void Add_Hl_De(uint16_t) { _register.hl += _register.de; _register.setFlagN(false);}
		void Ld_A_De(uint16_t) { _register.a = _memory[_register.pc + _register.de];}
		void Dec_De(uint16_t) {_register.de--;}
		void Inc_E(uint16_t) {_register.e++;}
		void Dec_E(uint16_t) {_register.e--;}
		void Ld_E(uint16_t arg) {_register.e = arg;}
		void Rra(uint16_t) {
			auto ci = _register.getFlagC() ? 0b10000000 : 0;
			auto co = (_register.a & 0b10000000) != 0;
			_register.a = (_register.a >> 1) + ci;
			_register.setFlagC(co);
		}


		void Jr_Nz(uint16_t arg) { /* 0x20 */
			if (!_register.getFlagN())
				_register.pc += arg;
			else
				_register.m = -4;
		}
		void Ld_Hl(uint16_t arg) { _register.hl = arg;}
		void Ld_Hlpp_A(uint16_t) {_register.a = _memory[_register.hl]; _register.hl++;}
		void Inc_Hl(uint16_t) {_register.hl++;}
		void Inc_H(uint16_t) {_register.h++;}
		void Dec_H(uint16_t) {_register.h--;}
		void Ld_H(uint16_t arg) {_register.h = arg;}
		void Daa(uint16_t) {
			if (!_register.getFlagN()) {  // after an addition, adjust if (half-)carry occurred or if result is out of bounds
				if (_register.getFlagC() || _register.a > 0x99) {
					_register.a += 0x60;
					_register.setFlagC(true);
				}
				if (_register.getFlagH() || (_register.a & 0x0f) > 0x09)
					_register.a += 0x6;
			} else {  // after _register.a subtraction, only adjust if (half-)carry occurred
				if (_register.getFlagC())
					_register.a -= 0x60;
				if (_register.getFlagH())
					_register.a -= 0x6;
			}
			_register.setFlagZ(_register.a == 0);
			_register.setFlagH(false);
		}
		void Jr_Z(uint16_t arg) {
			if (_register.getFlagZ())
				_register.pc += arg;
			else
				_register.m = -4;
		}
		void Add_Hl_Hl(uint16_t) {
			size_t tmp = _register.hl + _register.hl;
			_register.setFlagN(false);
		}
		void Ld_A_Hlp(uint16_t) {}
		void Dec_Hl(uint16_t) {_register.hl--;}
		void Inc_L(uint16_t) {_register.hl++;}
		void Dec_L(uint16_t) {_register.l++;}
		void Ld_L(uint16_t arg) {_register.l = arg;}
		void Cpl(uint16_t) {_register.a = ~_register.a; _register.setFlagN(true); _register.setFlagH(true);}

		void Jr_Nc(uint16_t arg) {if (!_register.getFlagC()) _register.pc = _register.pc + arg; else _register.m -= 4;} /* 0x30 */
		void Ld_Sp(uint16_t arg) {_register.sp = arg;}
		void Ld_Hln_A(uint16_t) {_register.a = _memory[_register.hl]; _register.hl--;}
		void Inc_Sp(uint16_t) {_register.sp--;}
		void Inc_Hlp(uint16_t) {_memory[_register.hl]++; _register.setFlagN(false); _register.setFlagZ(_memory[_register.hl] == 0);}
		void Dec_Hlp(uint16_t) {_memory[_register.hl]--; _register.setFlagN(false); _register.setFlagZ(_memory[_register.hl] == 0);}
		void Ld_Hlp(uint16_t arg) {}
		void Scf(uint16_t arg) {}
		void Jr_C(uint16_t arg) {}
		void Add_Hl_Sp(uint16_t) {}
		void Ld_A_Hln(uint16_t) {}
		void Dec_Sp(uint16_t) {}
		void Inc_a(uint16_t) {}
		void Dec_A(uint16_t) {}
		void Ld_A(uint16_t arg) {}
		void Ccf(uint16_t) {}

		void Ld_B_B(uint16_t) {_register.b = _register.b;} /* 0x40 */
		void Ld_B_C(uint16_t) {_register.b = _register.c;}
		void Ld_B_D(uint16_t) {_register.b = _register.d;}
		void Ld_B_E(uint16_t) {_register.b = _register.e;}
		void Ld_B_H(uint16_t) {_register.b = _register.h;}
		void Ld_B_L(uint16_t) {_register.b = _register.l;}
		void Ld_B_HLp(uint16_t) {_register.b = _memory[_register.hl];}
		void Ld_B_A(uint16_t) {_register.c = _register.a;}
		void Ld_C_B(uint16_t) {_register.c = _register.b;}
		void Ld_C_C(uint16_t) {_register.c = _register.c;}
		void Ld_C_D(uint16_t) {_register.c = _register.d;}
		void Ld_C_E(uint16_t) {_register.c = _register.e;}
		void Ld_C_H(uint16_t) {_register.c = _register.h;}
		void Ld_C_L(uint16_t) {_register.c = _register.l;}
		void Ld_C_HLp(uint16_t) {_register.c = _memory[_register.hl];}
		void Ld_C_A(uint16_t) {_register.c = _register.a;}

		void Ld_D_B(uint16_t) {_register.d = _register.b;} /* 0x50 */
		void Ld_D_C(uint16_t) {_register.d = _register.c;}
		void Ld_D_D(uint16_t) {_register.d = _register.d;}
		void Ld_D_E(uint16_t) {_register.d = _register.e;}
		void Ld_D_H(uint16_t) {_register.d = _register.h;}
		void Ld_D_L(uint16_t) {_register.d = _register.l;}
		void Ld_D_HLp(uint16_t) {_register.d = _memory[_register.hl];;}
		void Ld_D_A(uint16_t) {_register.d = _register.a;}
		void Ld_E_B(uint16_t) {_register.d = _register.b;}
		void Ld_E_C(uint16_t) {_register.d = _register.c;}
		void Ld_E_D(uint16_t) {_register.d = _register.d;}
		void Ld_E_E(uint16_t) {_register.d = _register.e;}
		void Ld_E_H(uint16_t) {_register.d = _register.h;}
		void Ld_E_L(uint16_t) {_register.d = _register.l;}
		void Ld_E_HLp(uint16_t) {_register.d = _memory[_register.hl];}
		void Ld_E_A(uint16_t) {_register.d = _register.a;}

		void Ld_H_B(uint16_t) {_register.h = _register.b;} /* 0x60 */
		void Ld_H_C(uint16_t) {_register.h = _register.c;}
		void Ld_H_D(uint16_t) {_register.h = _register.d;}
		void Ld_H_E(uint16_t) {_register.h = _register.e;}
		void Ld_H_H(uint16_t) {_register.h = _register.h;}
		void Ld_H_L(uint16_t) {_register.h = _register.l;}
		void Ld_H_HLp(uint16_t) {_register.h = _memory[_register.hl];}
		void Ld_H_A(uint16_t) {_register.h = _register.a;}
		void Ld_L_B(uint16_t) {_register.l = _register.b;}
		void Ld_L_C(uint16_t) {_register.l = _register.c;}
		void Ld_L_D(uint16_t) {_register.l = _register.d;}
		void Ld_L_E(uint16_t) {_register.l = _register.e;}
		void Ld_L_H(uint16_t) {_register.l = _register.h;}
		void Ld_L_L(uint16_t) {_register.l = _register.l;}
		void Ld_L_HLp(uint16_t) {_register.l = _memory[_register.hl];}
		void Ld_L_A(uint16_t) {_register.l = _register.a;}

		void Ld_Hlp_B(uint16_t) {_memory[_register.hl] = _register.b;} /* 0x70 */
		void Ld_Hlp_C(uint16_t) {_memory[_register.hl] = _register.c;}
		void Ld_Hlp_D(uint16_t) {_memory[_register.hl] = _register.d;}
		void Ld_Hlp_E(uint16_t) {_memory[_register.hl] = _register.e;}
		void Ld_Hlp_H(uint16_t) {_memory[_register.hl] = _register.h;}
		void Ld_Hlp_L(uint16_t) {_memory[_register.hl] = _register.l;}
		void Halt(uint16_t) {std::cout << "enter cpu lower mode" << std::endl;}
		void Ld_Hlp_A(uint16_t) {_memory[_register.hl] = _register.a;}
		void Ld_A_B(uint16_t) {_register.a = _register.b ;}
		void Ld_A_C(uint16_t) {_register.a = _register.c ;}
		void Ld_A_D(uint16_t) {_register.a = _register.d ;}
		void Ld_A_E(uint16_t) {_register.a = _register.e ;}
		void Ld_A_H(uint16_t) {_register.a = _register.h ;}
		void Ld_A_L(uint16_t) {_register.a = _register.l ;}
		void Ld_A_HLp(uint16_t) {_register.a = _memory[_register.hl] ;}
		void Ld_A_A(uint16_t) {_register.a = _register.a ;}

		void Add_A_B(uint16_t) {_register.a += _register.b; _register.setFlagZ(_register.a == 0); _register.setFlagN(false);} /* 0x80 */
		void Add_A_C(uint16_t) {_register.a += _register.c; _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}
		void Add_A_D(uint16_t) {_register.a += _register.d; _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}
		void Add_A_E(uint16_t) {_register.a += _register.e; _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}
		void Add_A_H(uint16_t) {_register.a += _register.h; _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}
		void Add_A_L(uint16_t) {_register.a += _register.l; _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}
		void Add_A_HLp(uint16_t) {_register.a += _memory[_register.hl]; _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}
		void Add_A_A(uint16_t) {_register.a += _register.a + _register.getFlagC(); _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}
		void Adc_A_B(uint16_t) {_register.a += _register.b + _register.getFlagC(); _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}
		void Adc_A_C(uint16_t) {_register.a += _register.c + _register.getFlagC(); _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}
		void Adc_A_D(uint16_t) {_register.a += _register.d + _register.getFlagC(); _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}
		void Adc_A_E(uint16_t) {_register.a += _register.e + _register.getFlagC(); _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}
		void Adc_A_H(uint16_t) {_register.a += _register.h + _register.getFlagC(); _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}
		void Adc_A_L(uint16_t) {_register.a += _register.l + _register.getFlagC(); _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}
		void Adc_A_HLp(uint16_t) {_register.a += _memory[_register.hl] + _register.getFlagC(); _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}
		void Adc_A_A(uint16_t) {_register.a += _register.a + _register.getFlagC(); _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}

		void Sub_B(uint16_t) {} /* 0x90 */
		void Sub_C(uint16_t) {}
		void Sub_D(uint16_t) {}
		void Sub_E(uint16_t) {}
		void Sub_H(uint16_t) {}
		void Sub_L(uint16_t) {}
		void Sub_HLp(uint16_t) {}
		void Sub_A(uint16_t) {}
		void Sbc_A_B(uint16_t) {}
		void Sbc_A_C(uint16_t) {}
		void Sbc_A_D(uint16_t) {}
		void Sbc_A_E(uint16_t) {}
		void Sbc_A_H(uint16_t) {}
		void Sbc_A_L(uint16_t) {}
		void Sbc_A_HLp(uint16_t) {}
		void Sbc_A_A(uint16_t) {}

		void And_B(uint16_t) {} /* 0xA0 */
		void And_C(uint16_t) {}
		void And_D(uint16_t) {}
		void And_E(uint16_t) {}
		void And_H(uint16_t) {}
		void And_L(uint16_t) {}
		void And_HLp(uint16_t) {}
		void And_A(uint16_t) {}
		void Xor_B(uint16_t) {}
		void Xor_C(uint16_t) {}
		void Xor_D(uint16_t) {}
		void Xor_E(uint16_t) {}
		void Xor_H(uint16_t) {}
		void Xor_L(uint16_t) {}
		void Xor_HLp(uint16_t) {}
		void Xor_A(uint16_t) {}

		void Or_B(uint16_t) {} /* 0xB0 */
		void Or_C(uint16_t) {}
		void Or_D(uint16_t) {}
		void Or_E(uint16_t) {}
		void Or_H(uint16_t) {}
		void Or_L(uint16_t) {}
		void Or_HLp(uint16_t) {}
		void Or_A(uint16_t) {}
		void Cp_B(uint16_t) {}
		void Cp_C(uint16_t) {}
		void Cp_D(uint16_t) {}
		void Cp_E(uint16_t) {}
		void Cp_H(uint16_t) {}
		void Cp_L(uint16_t) {}
		void Cp_HLp(uint16_t) {}
		void Cp_A(uint16_t) {}

		void Ret_Nz(uint16_t) {} /* 0xC0 */
		void Pop_Bc(uint16_t) {}
		void Jp_Nz(uint16_t arg) {}
		void Jp(uint16_t arg) {}
		void Call_Nz(uint16_t arg) {}
		void Push_Bc(uint16_t) {}
		void Add_A(uint16_t arg) {}
		void Rst_00H(uint16_t) {}
		void Ret_z(uint16_t) {}
		void Ret(uint16_t) {}
		void Jp_z(uint16_t arg) {}
		void Prefix_Cb(uint16_t) {}
		void Call_Z(uint16_t) {}
		void Call(uint16_t arg) {}
		void Adc(uint16_t) {}
		void Rst_08H(uint16_t) {}

		void Ret_Nc(uint16_t) {} /* 0xD0 */
		void Pop_De(uint16_t) {}
		void Jp_Nc(uint16_t arg) {}
//		void Jp(uint16_t arg) {}
		void Call_Nc(uint16_t arg) {}
		void Push_De(uint16_t) {}
		void Sub(uint16_t arg) {}
		void Rst_10H(uint16_t) {}
		void Ret_c(uint16_t) {}
		void Reti(uint16_t) {}
		void Jp_c(uint16_t arg) {}
//		void Prefix_Cb(uint16_t) {}
		void Call_C(uint16_t) {}
//		void Call(uint16_t arg) {}
		void Sbc(uint16_t) {}
		void Rst_18H(uint16_t) {}

		void Ldh_A(uint16_t) {} /* 0xE0 */
		void Pop_Hl(uint16_t) {}
		void Ld_Cp(uint16_t arg) {}
//		void Jp(uint16_t arg) {}
//		void Call_Nz(uint16_t arg) {}
		void Push_Hl(uint16_t) {}
		void And(uint16_t arg) {}
		void Rst_20H(uint16_t) {}
		void And_sp(uint16_t arg) {}
		void Jp_Hlp(uint16_t) {}
		void Ld_a_A(uint16_t arg) {}
//		void Prefix_Cb(uint16_t) {}
//		void Call_Z(uint16_t) {}
//		void Call(uint16_t arg) {}
		void Xor(uint16_t) {}
		void Rst_28H(uint16_t) {}

		void Ldh_a_A(uint16_t) {} /* 0xF0 */
		void Pop_Af(uint16_t) {}
		void Ld_A_Cp(uint16_t arg) {}
		void Di(uint16_t arg) {}
//		void Call_Nz(uint16_t arg) {}
		void Push_Af(uint16_t) {}
		void Or(uint16_t arg) {}
		void Rst_30H(uint16_t) {}
		void Ld_HL_SP(uint16_t) {}
		void Ld_Sp_Hl(uint16_t) {}
		void Ld_A_p(uint16_t arg) {}
		void Ei(uint16_t) {}
//		void Call_Z(uint16_t) {}
//		void Call(uint16_t arg) {}
		void Cp(uint16_t) {}
		void Rst_38H(uint16_t) {}

		void default_operator(uint16_t) {std::cout << "unknown operand: " << managedInstruction[_memory[_register.pc]]._name << std::endl; exit(1);}

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
