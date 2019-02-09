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
		~Cpu() = default;

		bool gotSomethingToRead() const;
		/**
		 * Read instruction and call the right function
		 */
		void readInstruction();

	private:
		void pushStack(uint16_t arg) {
			_register.sp -= 2;
			_memory[_register.sp] =     (0b1111111100000000 & arg);
			_memory[_register.sp + 1] = ((0b0000000011111111 & arg) >> 8);
		}

		uint16_t popStack() {
			uint16_t ret{0};

			ret = _memory[_register.sp] << 8;
			ret += _memory[_register.sp + 1];
			_register.sp += 2;
			return ret;
		}

		void Rst_00H(uint16_t) {} /* call reset vector oO  function rsv() */
		void Rst_08H(uint16_t) {} /* call reset vector oO  function rsv() */
		void Rst_10H(uint16_t) {} /* call reset vector oO  function rsv() */
		void Rst_18H(uint16_t) {} /* call reset vector oO  function rsv() */
		void Rst_20H(uint16_t) {} /* call reset vector oO  function rsv() */
		void Rst_28H(uint16_t) {}
		void Rst_30H(uint16_t) {}
		void Rst_38H(uint16_t) {}

		void Ei(uint16_t) {} /* enable interupt */
		void Reti(uint16_t) {} /* enable sub routine */
		void Di(uint16_t) {} /* disable interrupt */
		void Prefix_Cb(uint16_t) {} /* Call Prefix CB */

		void Cpl(uint16_t) {_register.a = ~_register.a; _register.setFlagN(true); _register.setFlagH(true);}
		void Scf(uint16_t) {}
		void Ccf(uint16_t) {}

		void Nop(uint16_t) {std::cout << "DO nothing" << std::endl;}
		void Stop(uint16_t arg) { std::cout << "STOP: " << arg << std::endl << "enter slow mode, what to do ?" << arg << std::endl;}
		void Halt(uint16_t) {std::cout << "enter cpu lower mode" << std::endl;}

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
		void Rra(uint16_t) { auto ci = _register.getFlagC() ? 0b10000000 : 0; auto co = (_register.a & 0b10000000) != 0; _register.a = (_register.a >> 1) + ci; _register.setFlagC(co);}
		void Rla(uint16_t) {auto ci= _register.getFlagC(); auto co= (_register.a & 0b10000000) != 0; _register.a= (_register.a << 1) + ci; _register.setFlagC(co);}
		void Rrca(uint16_t) {auto ci = _register.a & 0b00000001 ? 0b10000000 : 0; auto co = _register.a & 0b00000001; _register.a = (_register.a >> 1) + ci;_register.setFlagC(co);}
		void Rlca(uint16_t) {auto ci = _register.a & 0b10000000 ? 1 : 0; _register.a = (_register.a << 1) + ci; _register.setFlagC(ci);}

		void Dec_De(uint16_t) {_register.de--;}
		void Dec_BC(uint16_t) {_register.bc--;}
		void Dec_Sp(uint16_t) {_register.sp--;}
		void Dec_Hl(uint16_t) {_register.hl--;}
		void Dec_Hlp(uint16_t) {_memory[_register.hl]--; _register.setFlagN(true); _register.setFlagZ(_memory[_register.hl] == 0);}
		void Dec_A(uint16_t) {_register.a--; _register.setFlagN(true); _register.setFlagZ(_register.a == 0);}
		void Dec_B(uint16_t) {_register.b--; _register.setFlagN(true); _register.setFlagZ(_register.b == 0);}
		void Dec_C(uint16_t) {_register.c--; _register.setFlagN(true); _register.setFlagZ(_register.c == 0);}
		void Dec_D(uint16_t) {_register.d--; _register.setFlagN(true); _register.setFlagZ(_register.d == 0);}
		void Dec_E(uint16_t) {_register.e--; _register.setFlagN(true); _register.setFlagZ(_register.e == 0);}
		void Dec_H(uint16_t) {_register.h--; _register.setFlagN(true); _register.setFlagZ(_register.h == 0);}
		void Dec_L(uint16_t) {_register.l--; _register.setFlagN(true); _register.setFlagZ(_register.l == 0);}

		void Inc_Hl(uint16_t) {_register.hl++;}
		void Inc_BC(uint16_t) {_register.bc++;}
		void Inc_DE(uint16_t) {_register.de++;}
		void Inc_Sp(uint16_t) {_register.sp++;}

		void Inc_Hlp(uint16_t) {_memory[_register.hl]++; _register.setFlagN(false); _register.setFlagZ(_memory[_register.hl] == 0);}
		void Inc_A(uint16_t) {_register.a++; _register.setFlagN(false); _register.setFlagZ(_register.a == 0);}
		void Inc_B(uint16_t) {_register.b++; _register.setFlagN(false); _register.setFlagZ(_register.b == 0);}
		void Inc_C(uint16_t) {_register.c++; _register.setFlagN(false); _register.setFlagZ(_register.c == 0);}
		void Inc_D(uint16_t) {_register.d++; _register.setFlagN(false); _register.setFlagZ(_register.d == 0);}
		void Inc_E(uint16_t) {_register.e++; _register.setFlagN(false); _register.setFlagZ(_register.e == 0);}
		void Inc_H(uint16_t) {_register.h++; _register.setFlagN(false); _register.setFlagZ(_register.h == 0);}
		void Inc_L(uint16_t) {_register.hl++; _register.setFlagN(false); _register.setFlagZ(_register.l == 0);}

		void Add_SP(uint16_t arg){_register.sp += arg         ; _register.setFlagN(false); _register.setFlagZ(false);}
		void Add_Hl_Hl(uint16_t) {_register.hl += _register.hl; _register.setFlagN(false);}
		void Add_Hl_Sp(uint16_t) {_register.hl += _register.sp; _register.setFlagN(false);}
		void Add_HL_BC(uint16_t) {_register.hl += _register.bc; _register.setFlagN(false);}
		void Add_Hl_De(uint16_t) {_register.hl += _register.de; _register.setFlagN(false);}

		void Add_A(uint16_t arg) {_register.a += arg;       _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}
		void Add_A_A(uint16_t) {_register.a += _register.a; _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}
		void Add_A_B(uint16_t) {_register.a += _register.b; _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}
		void Add_A_C(uint16_t) {_register.a += _register.c; _register.setFlagZ(_register.b == 0); _register.setFlagN(false);}
		void Add_A_D(uint16_t) {_register.a += _register.d; _register.setFlagZ(_register.c == 0); _register.setFlagN(false);}
		void Add_A_E(uint16_t) {_register.a += _register.e; _register.setFlagZ(_register.d == 0); _register.setFlagN(false);}
		void Add_A_H(uint16_t) {_register.a += _register.h; _register.setFlagZ(_register.e == 0); _register.setFlagN(false);}
		void Add_A_L(uint16_t) {_register.a += _register.l; _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}
		void Add_A_Hlp(uint16_t) {_register.a += _memory[_register.hl]; _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}

		void Adc_A_B(uint16_t) {_register.a += _register.b + _register.getFlagC(); _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}
		void Adc_A_C(uint16_t) {_register.a += _register.c + _register.getFlagC(); _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}
		void Adc_A_D(uint16_t) {_register.a += _register.d + _register.getFlagC(); _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}
		void Adc_A_E(uint16_t) {_register.a += _register.e + _register.getFlagC(); _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}
		void Adc_A_H(uint16_t) {_register.a += _register.h + _register.getFlagC(); _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}
		void Adc_A_L(uint16_t) {_register.a += _register.l + _register.getFlagC(); _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}
		void Adc_A_Hlp(uint16_t) {_register.a += _memory[_register.hl] + _register.getFlagC(); _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}
		void Adc_A_A(uint16_t) {_register.a += _register.a + _register.getFlagC(); _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}
		void Adc(uint16_t arg) {_register.a += arg + _register.getFlagC(); _register.setFlagZ(_register.a == 0); _register.setFlagN(false);}

		void Sub(uint16_t arg) {_register.setFlagC(arg > _register.a)        ; _register.a -= arg        ; _register.setFlagZ(_register.a == 0); _register.setFlagN(true);}
		void Sub_A(uint16_t)   {_register.setFlagC(_register.a > _register.a); _register.a -= _register.a; _register.setFlagZ(_register.a == 0); _register.setFlagN(true);}
		void Sub_B(uint16_t)   {_register.setFlagC(_register.b > _register.a); _register.a -= _register.b; _register.setFlagZ(_register.a == 0); _register.setFlagN(true);}
		void Sub_C(uint16_t)   {_register.setFlagC(_register.c > _register.a); _register.a -= _register.c; _register.setFlagZ(_register.a == 0); _register.setFlagN(true);}
		void Sub_D(uint16_t)   {_register.setFlagC(_register.d > _register.a); _register.a -= _register.d; _register.setFlagZ(_register.a == 0); _register.setFlagN(true);}
		void Sub_E(uint16_t)   {_register.setFlagC(_register.e > _register.a); _register.a -= _register.e; _register.setFlagZ(_register.a == 0); _register.setFlagN(true);}
		void Sub_H(uint16_t)   {_register.setFlagC(_register.h > _register.a); _register.a -= _register.h; _register.setFlagZ(_register.a == 0); _register.setFlagN(true);}
		void Sub_L(uint16_t)   {_register.setFlagC(_register.l > _register.a); _register.a -= _register.l; _register.setFlagZ(_register.a == 0); _register.setFlagN(true);}
		void Sub_Hlp(uint16_t) {_register.setFlagC(_memory[_register.hl] > _register.a); _register.a -= _memory[_register.hl]; _register.setFlagZ(_register.a == 0); _register.setFlagN(true);}

		void Sbc(uint16_t arg) {auto res = _register.getFlagC(); _register.setFlagC(_register.a > _register.a); _register.a -= arg - res; _register.setFlagZ(_register.a == 0); _register.setFlagN(true);}
		void Sbc_A_A(uint16_t) {auto res = _register.getFlagC(); _register.setFlagC(_register.a > _register.a); _register.a -= _register.a - res; _register.setFlagZ(_register.a == 0); _register.setFlagN(true);}
		void Sbc_A_B(uint16_t) {auto res = _register.getFlagC(); _register.setFlagC(_register.b > _register.a); _register.a -= _register.b - res; _register.setFlagZ(_register.a == 0); _register.setFlagN(true);}
		void Sbc_A_C(uint16_t) {auto res = _register.getFlagC(); _register.setFlagC(_register.c > _register.a); _register.a -= _register.c - res; _register.setFlagZ(_register.a == 0); _register.setFlagN(true);}
		void Sbc_A_D(uint16_t) {auto res = _register.getFlagC(); _register.setFlagC(_register.d > _register.a); _register.a -= _register.d - res; _register.setFlagZ(_register.a == 0); _register.setFlagN(true);}
		void Sbc_A_E(uint16_t) {auto res = _register.getFlagC(); _register.setFlagC(_register.e > _register.a); _register.a -= _register.e - res; _register.setFlagZ(_register.a == 0); _register.setFlagN(true);}
		void Sbc_A_H(uint16_t) {auto res = _register.getFlagC(); _register.setFlagC(_register.h > _register.a); _register.a -= _register.h - res; _register.setFlagZ(_register.a == 0); _register.setFlagN(true);}
		void Sbc_A_L(uint16_t) {auto res = _register.getFlagC(); _register.setFlagC(_register.l > _register.a); _register.a -= _register.l - res; _register.setFlagZ(_register.a == 0); _register.setFlagN(true);}
		void Sbc_A_Hlp(uint16_t) {auto res = _register.getFlagC(); _register.setFlagC(_memory[_register.hl] > _register.a); _register.a -= _memory[_register.hl] - res; _register.setFlagZ(_register.a == 0); _register.setFlagN(true);}

		/* 0xA0 */
		void And(uint16_t arg) {_register.a &= arg; _register.setFlagZ(_register.a == 0); _register.setFlagN(false); _register.setFlagH(true); _register.setFlagC(false);}
		void And_A(uint16_t) {_register.a &= _register.a; _register.setFlagZ(_register.a == 0); _register.setFlagN(false); _register.setFlagH(true); _register.setFlagC(false);}
		void And_B(uint16_t) {_register.a &= _register.b; _register.setFlagZ(_register.a == 0); _register.setFlagN(false); _register.setFlagH(true); _register.setFlagC(false);}
		void And_C(uint16_t) {_register.a &= _register.c; _register.setFlagZ(_register.a == 0); _register.setFlagN(false); _register.setFlagH(true); _register.setFlagC(false);}
		void And_D(uint16_t) {_register.a &= _register.d; _register.setFlagZ(_register.a == 0); _register.setFlagN(false); _register.setFlagH(true); _register.setFlagC(false);}
		void And_E(uint16_t) {_register.a &= _register.e; _register.setFlagZ(_register.a == 0); _register.setFlagN(false); _register.setFlagH(true); _register.setFlagC(false);}
		void And_H(uint16_t) {_register.a &= _register.h; _register.setFlagZ(_register.a == 0); _register.setFlagN(false); _register.setFlagH(true); _register.setFlagC(false);}
		void And_L(uint16_t) {_register.a &= _register.l; _register.setFlagZ(_register.a == 0); _register.setFlagN(false); _register.setFlagH(true); _register.setFlagC(false);}
		void And_Hlp(uint16_t) {_register.a &= _memory[_register.hl]; _register.setFlagZ(_register.a == 0); _register.setFlagN(false); _register.setFlagH(true); _register.setFlagC(false);}

		void Xor(uint16_t arg) {_register.a ^= arg; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Xor_B(uint16_t) {_register.a ^= _register.b; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Xor_C(uint16_t) {_register.a ^= _register.c; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Xor_D(uint16_t) {_register.a ^= _register.d; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Xor_E(uint16_t) {_register.a ^= _register.e; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Xor_H(uint16_t) {_register.a ^= _register.h; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Xor_L(uint16_t) {_register.a ^= _register.l; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Xor_Hlp(uint16_t) {_register.a ^= _memory[_register.hl]; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Xor_A(uint16_t) {_register.a ^= _register.a; _register.f = 0; _register.setFlagZ(_register.a == 0);}

		void Or(uint16_t arg) {_register.a |= arg; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Or_A(uint16_t) {_register.a |= _register.a; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Or_B(uint16_t) {_register.a |= _register.b; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Or_C(uint16_t) {_register.a |= _register.c; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Or_D(uint16_t) {_register.a |= _register.d; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Or_E(uint16_t) {_register.a |= _register.e; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Or_H(uint16_t) {_register.a |= _register.h; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Or_L(uint16_t) {_register.a |= _register.l; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Or_Hlp(uint16_t) {_register.a |= _memory[_register.hl]; _register.f = 0; _register.setFlagZ(_register.a == 0);}

		void Cp(uint16_t arg) {_register.setFlagC(arg > _register.a); _register.setFlagN(true); _register.setFlagZ(_register.a == arg); }
		void Cp_A(uint16_t) {_register.setFlagC(false); _register.setFlagN(true); _register.setFlagZ(true);}
		void Cp_B(uint16_t) {_register.setFlagC(_register.b > _register.a); _register.setFlagN(true); _register.setFlagZ(_register.b == _register.a);}
		void Cp_C(uint16_t) {_register.setFlagC(_register.c > _register.a); _register.setFlagN(true); _register.setFlagZ(_register.c == _register.a);}
		void Cp_D(uint16_t) {_register.setFlagC(_register.d > _register.a); _register.setFlagN(true); _register.setFlagZ(_register.d == _register.a);}
		void Cp_E(uint16_t) {_register.setFlagC(_register.e > _register.a); _register.setFlagN(true); _register.setFlagZ(_register.e == _register.a);}
		void Cp_H(uint16_t) {_register.setFlagC(_register.h > _register.a); _register.setFlagN(true); _register.setFlagZ(_register.h == _register.a);}
		void Cp_L(uint16_t) {_register.setFlagC(_register.l > _register.a); _register.setFlagN(true); _register.setFlagZ(_register.l == _register.a);}
		void Cp_Hlp(uint16_t) {_register.setFlagC(_memory[_register.hl] > _register.a); _register.setFlagN(true); _register.setFlagZ(_memory[_register.hl] == _register.a);}

		void Call(uint16_t arg)    {                            this->pushStack(_register.pc + 2); _register.pc = arg;}
		void Call_C(uint16_t arg)  {if (_register.getFlagC())  {this->pushStack(_register.pc + 2); _register.pc = arg;} else _register.m -= 3;}
		void Call_Z(uint16_t arg)  {if (_register.getFlagZ())  {this->pushStack(_register.pc + 2); _register.pc = arg;} else _register.m -= 3;}
		void Call_Nc(uint16_t arg) {if (!_register.getFlagC()) {this->pushStack(_register.pc + 2); _register.pc = arg;} else _register.m -= 3;}
		void Call_Nz(uint16_t arg) {if (!_register.getFlagZ()) {this->pushStack(_register.pc + 2); _register.pc = arg;} else _register.m -= 3;}

		void Push_De(uint16_t) {this->pushStack(_register.de);}
		void Push_Hl(uint16_t) {this->pushStack(_register.hl);}
		void Push_Bc(uint16_t) {this->pushStack(_register.bc);}
		void Push_Af(uint16_t) {this->pushStack(_register.af);}

		void Pop_De(uint16_t) {_register.de = this->popStack();}
		void Pop_Bc(uint16_t) {_register.bc = this->popStack();}
		void Pop_Af(uint16_t) {_register.af = this->popStack();}
		void Pop_Hl(uint16_t) {_register.hl = this->popStack();}

		void Ret(uint16_t)    {_register.pc = this->popStack();}
		void Ret_Nz(uint16_t) {if (!_register.getFlagZ()) _register.pc = this->popStack(); else _register.m -= 1;}
		void Ret_Nc(uint16_t) {if (!_register.getFlagC()) _register.pc = this->popStack(); else _register.m -= 3;}
		void Ret_Z(uint16_t)  {if (_register.getFlagZ())  _register.pc = this->popStack(); else _register.m -= 1;}
		void Ret_C(uint16_t)  {if (_register.getFlagC())  _register.pc = this->popStack(); else _register.m -= 3;}

		void Jp(uint16_t arg)    {_register.pc = arg;}
		void Jp_Hlp(uint16_t)    {_register.pc = _memory[_register.hl];}
		void Jp_Nz(uint16_t arg) {if (!_register.getFlagZ()) _register.pc = arg; else _register.m -= 1;}
		void Jp_Nc(uint16_t arg) {if (!_register.getFlagC()) _register.pc = arg; else _register.m -= 1;}
		void Jp_Z(uint16_t arg)  {if (_register.getFlagZ())  _register.pc = arg; else _register.m -= 1;}
		void Jp_c(uint16_t arg)  {if (_register.getFlagC())  _register.pc = arg; else _register.m -= 1;}

		void Jr(uint16_t arg)    {_register.pc += arg;}
		void Jr_Nz(uint16_t arg) {if (!_register.getFlagN()) _register.pc += arg; else _register.m -= 1;}
		void Jr_Nc(uint16_t arg) {if (!_register.getFlagC()) _register.pc += arg; else _register.m -= 1;}
		void Jr_Z(uint16_t arg)  {if (_register.getFlagZ())  _register.pc += arg; else _register.m -= 1;}
		void Jr_C(uint16_t arg)  {if (_register.getFlagC())  _register.pc += arg; else _register.m -= 1;}

		void Ld_SP(uint16_t arg) {_memory[_register.pc + arg] = _register.sp;}
		void Ld_BC(uint16_t arg) {_register.bc = arg;}
		void Ld_BC_A(uint16_t) {_memory[_register.pc + _register.bc] = _register.a;}
		void Ld_DE(uint16_t arg) {_register.de = arg;}
		void Ld_DE_A(uint16_t) {_memory[_register.pc + _register.de] = _register.a;}
		void Ld_Hlpp_A(uint16_t) {_register.a = _memory[_register.hl]; _register.hl++;}
		void Ld_HL_SP(uint16_t arg) {size_t res = _register.sp + arg; _register.hl = res & 255; _register.setFlagZ(false); _register.setFlagN(false);}
		void Ld_Sp_Hl(uint16_t) {_register.sp = _register.hl;}
		void Ld_Cp(uint16_t arg) {_memory[_register.c] = arg;}

		void Ld_L(uint16_t arg) {_register.l = arg;}
		void Ld_Sp(uint16_t arg) {_register.sp = arg;}
		void Ld_Hl(uint16_t arg) { _register.hl = arg;}
		void Ld_Hln_A(uint16_t) {_register.a = _memory[_register.hl]; _register.hl--;}
		void Ld_Hlp(uint16_t arg) {_memory[_register.hl] = arg;}

		void Ldh_a_A(uint16_t) {_memory[_register.hl] = _register.a;}
		void Ldh_A(uint16_t arg) {_memory[arg] = _register.a;}
		void Ld_A_p(uint16_t arg) {_register.a = _memory[arg];}
		void Ld_a_A(uint16_t arg) {_memory[arg] = _register.a;}
		void Ld_A(uint16_t arg) {_register.a = arg;}
		void Ld_A_A(uint16_t) {_register.a = _register.a ;}
		void Ld_A_B(uint16_t) {_register.a = _register.b ;}
		void Ld_A_C(uint16_t) {_register.a = _register.c ;}
		void Ld_A_D(uint16_t) {_register.a = _register.d ;}
		void Ld_A_E(uint16_t) {_register.a = _register.e ;}
		void Ld_A_H(uint16_t) {_register.a = _register.h ;}
		void Ld_A_L(uint16_t) {_register.a = _register.l ;}
		void Ld_A_Cp(uint16_t) {_register.a = _memory[_register.c];}
		void Ld_A_BC(uint16_t) {_register.a = _memory[_register.pc + _register.bc];}
		void Ld_A_De(uint16_t) { _register.a = _memory[_register.pc + _register.de];}
		void Ld_A_Hln(uint16_t) {_register.a = _memory[_register.hl];}
		void Ld_A_Hlpp(uint16_t) {_register.a = _memory[_register.hl];}
		void Ld_A_Hlp(uint16_t) {_register.a = _memory[_register.hl] ;}

		void Ld_B(uint16_t) {_register.b = _memory[_register.pc];};
		void Ld_B_B(uint16_t) {_register.b = _register.b;} /* 0x40 */
		void Ld_B_C(uint16_t) {_register.b = _register.c;}
		void Ld_B_D(uint16_t) {_register.b = _register.d;}
		void Ld_B_E(uint16_t) {_register.b = _register.e;}
		void Ld_B_H(uint16_t) {_register.b = _register.h;}
		void Ld_B_L(uint16_t) {_register.b = _register.l;}
		void Ld_B_Hlp(uint16_t) {_register.b = _memory[_register.hl];}
		void Ld_B_A(uint16_t) {_register.c = _register.a;}

		void Ld_C(uint16_t arg) {_register.c = arg;}
		void Ld_C_B(uint16_t) {_register.c = _register.b;}
		void Ld_C_C(uint16_t) {_register.c = _register.c;}
		void Ld_C_D(uint16_t) {_register.c = _register.d;}
		void Ld_C_E(uint16_t) {_register.c = _register.e;}
		void Ld_C_H(uint16_t) {_register.c = _register.h;}
		void Ld_C_L(uint16_t) {_register.c = _register.l;}
		void Ld_C_Hlp(uint16_t) {_register.c = _memory[_register.hl];}
		void Ld_C_A(uint16_t) {_register.c = _register.a;}

		void Ld_D(uint16_t arg) {_register.e = arg;}
		void Ld_D_B(uint16_t) {_register.d = _register.b;} /* 0x50 */
		void Ld_D_C(uint16_t) {_register.d = _register.c;}
		void Ld_D_D(uint16_t) {_register.d = _register.d;}
		void Ld_D_E(uint16_t) {_register.d = _register.e;}
		void Ld_D_H(uint16_t) {_register.d = _register.h;}
		void Ld_D_L(uint16_t) {_register.d = _register.l;}
		void Ld_D_Hlp(uint16_t) {_register.d = _memory[_register.hl];}
		void Ld_D_A(uint16_t) {_register.d = _register.a;}

		void Ld_E(uint16_t arg) {_register.e = arg;}
		void Ld_E_B(uint16_t) {_register.d = _register.b;}
		void Ld_E_C(uint16_t) {_register.d = _register.c;}
		void Ld_E_D(uint16_t) {_register.d = _register.d;}
		void Ld_E_E(uint16_t) {_register.d = _register.e;}
		void Ld_E_H(uint16_t) {_register.d = _register.h;}
		void Ld_E_L(uint16_t) {_register.d = _register.l;}
		void Ld_E_Hlp(uint16_t) {_register.d = _memory[_register.hl];}
		void Ld_E_A(uint16_t) {_register.d = _register.a;}

		void Ld_H(uint16_t arg) {_register.h = arg;}
		void Ld_H_B(uint16_t) {_register.h = _register.b;} /* 0x60 */
		void Ld_H_C(uint16_t) {_register.h = _register.c;}
		void Ld_H_D(uint16_t) {_register.h = _register.d;}
		void Ld_H_E(uint16_t) {_register.h = _register.e;}
		void Ld_H_H(uint16_t) {_register.h = _register.h;}
		void Ld_H_L(uint16_t) {_register.h = _register.l;}
		void Ld_H_Hlp(uint16_t) {_register.h = _memory[_register.hl];}
		void Ld_H_A(uint16_t) {_register.h = _register.a;}


		void Ld_L_B(uint16_t) {_register.l = _register.b;}
		void Ld_L_C(uint16_t) {_register.l = _register.c;}
		void Ld_L_D(uint16_t) {_register.l = _register.d;}
		void Ld_L_E(uint16_t) {_register.l = _register.e;}
		void Ld_L_H(uint16_t) {_register.l = _register.h;}
		void Ld_L_L(uint16_t) {_register.l = _register.l;}
		void Ld_L_Hlp(uint16_t) {_register.l = _memory[_register.hl];}
		void Ld_L_A(uint16_t) {_register.l = _register.a;}

		void Ld_Hlp_B(uint16_t) {_memory[_register.hl] = _register.b;} /* 0x70 */
		void Ld_Hlp_C(uint16_t) {_memory[_register.hl] = _register.c;}
		void Ld_Hlp_D(uint16_t) {_memory[_register.hl] = _register.d;}
		void Ld_Hlp_E(uint16_t) {_memory[_register.hl] = _register.e;}
		void Ld_Hlp_H(uint16_t) {_memory[_register.hl] = _register.h;}
		void Ld_Hlp_L(uint16_t) {_memory[_register.hl] = _register.l;}
		void Ld_Hlp_A(uint16_t) {_memory[_register.hl] = _register.a;}

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
