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
		struct prefixInfos
		{
			std::string _name;
			void (Cpu::*_instruction)();
		};

		explicit Cpu(std::vector<uint8_t> &instuctions);
		~Cpu() = default;

		bool gotSomethingToRead() const;
		/**
		 * Read instruction and call the right function
		 */
		void readInstruction();
	private:
		static std::vector<instructionInfos> managedInstruction;
		static std::vector<prefixInfos> prefixInstruction;
		Register _register;
		Clock _timer;
		Memory _memory;
		gfx::Screen _gpu;

		size_t _romSize;
		bool _read;

		void r(uint8_t &value) {
			_register.f = (value & 0b00000001 ? 0b00010000 : 0b00000000); // set all to zero and C to the oc
			value = (value >> 1);
		}

		void l(uint8_t &value){
			_register.f = (value & 0b10000000 ? 0b00010000 : 0b00000000);  // set all to zero and C to the oc
			value = (value << 1);
		}

		void rr(uint8_t &value) {
			auto ci = _register.getFlagC() ? 0b10000000 : 0;
			r(value);
			value += ci;
		}

		void rl(uint8_t &value) {
			auto tmp =  _register.getFlagC();
			l(value);
			value +=  tmp;
		}

		void rrc(uint8_t &value) {
			auto co = (value & 0b00000001) << 7;
			r(value);
			value += co << 7;
		}

		void rlc(uint8_t &value) {
			auto co = (value & 0b10000000) >> 7 ;
			l(value);
			value += co;
		}

		void pushStack(uint16_t arg) {
			_register.sp--;
			_memory[_register.sp] = (0b1111111100000000 & arg) >> 8;
			_register.sp--;
			_memory[_register.sp] = (0b0000000011111111 & arg);
		}

		uint16_t popStack() {
			uint16_t ret{0};

			ret = _memory[_register.sp];
			_register.sp++;
			ret += _memory[_register.sp] << 8;
			_register.sp++;
			return ret;
		}

		uint16_t getStack(uint8_t value=0) {
			uint16_t ret{0};

			ret = _memory[_register.sp + value * 2];
			ret += _memory[_register.sp + value * 2 + 1]  << 8;
			return ret;
		}


		void Rst_00H(uint16_t) {_register.iem = 0; this->pushStack(_register.pc); _register.pc = 0x00;}
		void Rst_08H(uint16_t) {_register.iem = 0; this->pushStack(_register.pc); _register.pc = 0x08;}
		void Rst_10H(uint16_t) {_register.iem = 0; this->pushStack(_register.pc); _register.pc = 0x10;}
		void Rst_18H(uint16_t) {_register.iem = 0; this->pushStack(_register.pc); _register.pc = 0x18;}
		void Rst_20H(uint16_t) {_register.iem = 0; this->pushStack(_register.pc); _register.pc = 0x20;}
		void Rst_28H(uint16_t) {_register.iem = 0; this->pushStack(_register.pc); _register.pc = 0x28;}
		void Rst_30H(uint16_t) {_register.iem = 0; this->pushStack(_register.pc); _register.pc = 0x30;}
		void Rst_38H(uint16_t) {_register.iem = 0; this->pushStack(_register.pc); _register.pc = 0x38;}
		void Rst_40H(uint16_t) {_register.iem = 0; this->pushStack(_register.pc); _register.pc = 0x40;}
		void Rst_48H(uint16_t) {_register.iem = 0; this->pushStack(_register.pc); _register.pc = 0x48;}
		void Rst_50H(uint16_t) {_register.iem = 0; this->pushStack(_register.pc); _register.pc = 0x50;}
		void Rst_58H(uint16_t) {_register.iem = 0; this->pushStack(_register.pc); _register.pc = 0x58;}
		void Rst_60H(uint16_t) {_register.iem = 0; this->pushStack(_register.pc); _register.pc = 0x60;}


		void Reti(uint16_t) {_register.iem = 1; _register.pc = this->popStack();}       /* enable sub routine */
		void Ei(uint16_t) {_register.iem = 1;}         /* enable interupt */
		void Di(uint16_t) {_register.iem = 0;}         /* disable interrupt */
		void Prefix_Cb(uint16_t arg) {(*this.*prefixInstruction[arg]._instruction)();}  /* Call Prefix CB */

		void Cpl(uint16_t) {_register.a = ~_register.a; _register.setFlagN(true); _register.setFlagH(true);}
		void Scf(uint16_t) {_register.setFlagN(false); _register.setFlagH(false); _register.setFlagC(true);}
		void Ccf(uint16_t) {_register.setFlagN(false); _register.setFlagH(false); _register.setFlagC(!_register.getFlagC());}

		void Nop(uint16_t) {} // {std::cout << "DO nothing" << std::endl;}
		void Stop(uint16_t arg) { std::cout << "STOP: " << arg << std::endl << "enter slow mode, what to do ?" << arg << std::endl;_memory.dumpMemory(0x8000, 0x9FFF);}
		void Halt(uint16_t) {std::cout << "enter cpu lower mode" << std::endl;}

		void Daa(uint16_t) {
			uint8_t value = 0;

			if (!_register.getFlagN()) {  // after an addition, adjust if (half-)carry occurred or if result is out of bounds
				if (_register.getFlagC() || value > 0x99) {
					value += 0x60;
					_register.setFlagC(true);
				}
				if (_register.getFlagH() || (value & 0x0f) > 0x09)
					value += 0x6;
			} else {  // after _register.a subtraction, only adjust if (half-)carry occurred
				if (_register.getFlagC())
					value -= 0x60;
				if (_register.getFlagH())
					value -= 0x6;
			}
			_register.setFlagZ(value == 0 ? _register.getFlagZ() : false);
			_register.setFlagH(false);
			_register.a = value;
		}
		void Rra(uint16_t) {rr(_register.a);}
		void Rrca(uint16_t) {rrc(_register.a);}

		void Rla(uint16_t) {rl(_register.a);}
		void Rlca(uint16_t) {rlc(_register.a);}

		void Dec_De(uint16_t) {_register.de--;}
		void Dec_BC(uint16_t) {_register.bc--;}
		void Dec_Sp(uint16_t) {_register.sp--;}
		void Dec_Hl(uint16_t) {_register.hl--;}

		void Dec_reg(uint8_t &reg) {
			uint8_t res = reg - 1;

			_register.setFlagN(true);
			_register.setFlagZ(res == 0);
			_register.setFlagH((reg & 0xF) < (res & 0xF));

			reg = res;
		}
		void Dec_Hlp(uint16_t) {Dec_reg(_memory[_register.hl]);}
		void Dec_A(uint16_t) {Dec_reg(_register.a);}
		void Dec_B(uint16_t) {Dec_reg(_register.b);}
		void Dec_C(uint16_t) {Dec_reg(_register.c);}
		void Dec_D(uint16_t) {Dec_reg(_register.d);}
		void Dec_E(uint16_t) {Dec_reg(_register.e);}
		void Dec_H(uint16_t) {Dec_reg(_register.h);}
		void Dec_L(uint16_t) {Dec_reg(_register.l);}

		void Inc_Hl(uint16_t) {_register.hl++;}
		void Inc_BC(uint16_t) {_register.bc++;}
		void Inc_DE(uint16_t) {_register.de++;}
		void Inc_Sp(uint16_t) {_register.sp++;}

		void Inc_reg(uint8_t &reg) {
			uint8_t res = reg + 1;

			_register.setFlagN(false);
			_register.setFlagZ(res == 0);
			_register.setFlagH((uint8_t(reg) & 0xF) + (1 & 0xF) > 0xF);

			reg = res;
		}
		void Inc_Hlp(uint16_t) {_register.setFlagH((_memory[_register.hl] & 0xF) + (1 & 0xF) > 0xF);_memory[_register.hl]++; _register.setFlagN(false); _register.setFlagZ(_memory[_register.hl] == 0);}
		void Inc_A(uint16_t)   {Inc_reg(_register.a);}
		void Inc_B(uint16_t)   {Inc_reg(_register.b);}
		void Inc_C(uint16_t)   {Inc_reg(_register.c);}
		void Inc_D(uint16_t)   {Inc_reg(_register.d);}
		void Inc_E(uint16_t)   {Inc_reg(_register.e);}
		void Inc_H(uint16_t)   {Inc_reg(_register.h);}
		void Inc_L(uint16_t)   {Inc_reg(_register.l);}

		void Add_SP(uint16_t arg){
			_register.setFlagH((uint8_t(_register.sp) & 0xF) + (arg & 0xF) > 0xF);
			_register.setFlagC((_register.sp & 0xFF) + int(arg) > 0xFF);
			_register.sp += (int8_t)arg ;
			_register.setFlagN(false);
			_register.setFlagZ(false);
		}

		void Add_hl_ref(uint16_t to_add) {
			auto res = _register.hl + to_add;

			_register.setFlagN(false);
			_register.setFlagH((_register.hl & 0xFFF) + (to_add & 0xFFF) > 0xFFF);
			_register.setFlagC(to_add > 0 && _register.hl > res);

			_register.hl = res;
		}
		void Add_Hl_Hl(uint16_t) {Add_hl_ref(_register.hl);}
		void Add_Hl_Sp(uint16_t) {Add_hl_ref(_register.sp);}
		void Add_HL_BC(uint16_t) {Add_hl_ref(_register.bc);}
		void Add_Hl_De(uint16_t) {Add_hl_ref(_register.de);}

		void Add_ref_ref(uint8_t &value, uint16_t to_add, bool carry) {
			uint8_t res = value + to_add;
			carry = carry && _register.getFlagC();

			_register.setFlagH(((int(value) & 0xF) + (to_add & 0xF)) > 0xF);
			_register.setFlagC(to_add > 0 && value > res);
			if (carry) {
				_register.setFlagH((res & 0xF) == 0xF);
				_register.setFlagC(res == 0xFF);
				res++;
			}
			value = res;
			_register.setFlagZ(res == 0);
			_register.setFlagN(false);
		}
		void Add_A(uint16_t arg) {Add_ref_ref(_register.a, arg, false);}
		void Add_A_A(uint16_t)   {Add_ref_ref(_register.a, _register.a, false);}
		void Add_A_B(uint16_t)   {Add_ref_ref(_register.a, _register.b, false);}
		void Add_A_C(uint16_t)   {Add_ref_ref(_register.a, _register.c, false);}
		void Add_A_D(uint16_t)   {Add_ref_ref(_register.a, _register.d, false);}
		void Add_A_E(uint16_t)   {Add_ref_ref(_register.a, _register.e, false);}
		void Add_A_H(uint16_t)   {Add_ref_ref(_register.a, _register.h, false);}
		void Add_A_L(uint16_t)   {Add_ref_ref(_register.a, _register.l, false);}
		void Add_A_Hlp(uint16_t) {Add_ref_ref(_register.a, _memory[_register.hl], false);}

		void Adc_A_B(uint16_t)   { Add_ref_ref(_register.a, _register.b, true);}
		void Adc_A_C(uint16_t)   { Add_ref_ref(_register.a, _register.c, true);}
		void Adc_A_D(uint16_t)   { Add_ref_ref(_register.a, _register.d, true);}
		void Adc_A_E(uint16_t)   { Add_ref_ref(_register.a, _register.e, true);}
		void Adc_A_H(uint16_t)   { Add_ref_ref(_register.a, _register.h, true);}
		void Adc_A_L(uint16_t)   { Add_ref_ref(_register.a, _register.l, true);}
		void Adc_A_Hlp(uint16_t) { Add_ref_ref(_register.a, _memory[_register.hl], true); }
		void Adc_A_A(uint16_t)   { Add_ref_ref(_register.a, _register.a, true);}
		void Adc(uint16_t arg)   { Add_ref_ref(_register.a, arg, true);}


		void Sub_ref_ref(uint8_t &value, uint16_t to_sub, bool carry) {
			uint8_t res = value - to_sub;
			carry  = carry && _register.getFlagC();

			_register.setFlagH((value & 0xF) < (to_sub & 0xF));
			_register.setFlagC(to_sub > value);

			if  (carry) {
				_register.setFlagH((res & 0xF) == 0);
				_register.setFlagC(res == 0);
				res--;
			}

			_register.setFlagZ(res == 0);
			_register.setFlagN(true);
			value = res;
		}
		void Sub(uint16_t arg) {Sub_ref_ref(_register.a, arg, false);                           }
		void Sub_A(uint16_t)   {Sub_ref_ref(_register.a, _register.a , false);                  }
		void Sub_B(uint16_t)   {Sub_ref_ref(_register.a, _register.b , false);                  }
		void Sub_C(uint16_t)   {Sub_ref_ref(_register.a, _register.c , false);                  }
		void Sub_D(uint16_t)   {Sub_ref_ref(_register.a, _register.d , false);                  }
		void Sub_E(uint16_t)   {Sub_ref_ref(_register.a, _register.e , false);                  }
		void Sub_H(uint16_t)   {Sub_ref_ref(_register.a, _register.h , false);                  }
		void Sub_L(uint16_t)   {Sub_ref_ref(_register.a, _register.l , false);                  }
		void Sub_Hlp(uint16_t) {Sub_ref_ref(_register.a, _memory[_register.hl] , false);}

		void Sbc(uint16_t arg)   {Sub_ref_ref(_register.a, arg, true);                   }
		void Sbc_A_A(uint16_t)   {Sub_ref_ref(_register.a, _register.a , true);          }
		void Sbc_A_B(uint16_t)   {Sub_ref_ref(_register.a, _register.b , true);          }
		void Sbc_A_C(uint16_t)   {Sub_ref_ref(_register.a, _register.c , true);          }
		void Sbc_A_D(uint16_t)   {Sub_ref_ref(_register.a, _register.d , true);          }
		void Sbc_A_E(uint16_t)   {Sub_ref_ref(_register.a, _register.e , true);          }
		void Sbc_A_H(uint16_t)   {Sub_ref_ref(_register.a, _register.h , true);          }
		void Sbc_A_L(uint16_t)   {Sub_ref_ref(_register.a, _register.l , true);          }
		void Sbc_A_Hlp(uint16_t) {Sub_ref_ref(_register.a, _memory[_register.hl] , true);}

		/* 0xA0 */
		void And(uint16_t arg) {_register.a &= arg;         _register.setFlagZ(_register.a == 0); _register.setFlagN(false); _register.setFlagH(true); _register.setFlagC(false);}
		void And_A(uint16_t)   {_register.a &= _register.a; _register.setFlagZ(_register.a == 0); _register.setFlagN(false); _register.setFlagH(true); _register.setFlagC(false);}
		void And_B(uint16_t)   {_register.a &= _register.b; _register.setFlagZ(_register.a == 0); _register.setFlagN(false); _register.setFlagH(true); _register.setFlagC(false);}
		void And_C(uint16_t)   {_register.a &= _register.c; _register.setFlagZ(_register.a == 0); _register.setFlagN(false); _register.setFlagH(true); _register.setFlagC(false);}
		void And_D(uint16_t)   {_register.a &= _register.d; _register.setFlagZ(_register.a == 0); _register.setFlagN(false); _register.setFlagH(true); _register.setFlagC(false);}
		void And_E(uint16_t)   {_register.a &= _register.e; _register.setFlagZ(_register.a == 0); _register.setFlagN(false); _register.setFlagH(true); _register.setFlagC(false);}
		void And_H(uint16_t)   {_register.a &= _register.h; _register.setFlagZ(_register.a == 0); _register.setFlagN(false); _register.setFlagH(true); _register.setFlagC(false);}
		void And_L(uint16_t)   {_register.a &= _register.l; _register.setFlagZ(_register.a == 0); _register.setFlagN(false); _register.setFlagH(true); _register.setFlagC(false);}
		void And_Hlp(uint16_t) {_register.a &= _memory[_register.hl]; _register.setFlagZ(_register.a == 0); _register.setFlagN(false); _register.setFlagH(true); _register.setFlagC(false);}

		void Xor(uint16_t arg) {_register.a ^= arg;         _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Xor_B(uint16_t)   {_register.a ^= _register.b; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Xor_C(uint16_t)   {_register.a ^= _register.c; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Xor_D(uint16_t)   {_register.a ^= _register.d; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Xor_E(uint16_t)   {_register.a ^= _register.e; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Xor_H(uint16_t)   {_register.a ^= _register.h; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Xor_L(uint16_t)   {_register.a ^= _register.l; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Xor_Hlp(uint16_t) {_register.a ^= _memory[_register.hl]; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Xor_A(uint16_t)   {_register.a ^= _register.a; _register.f = 0; _register.setFlagZ(_register.a == 0);}

		void Or(uint16_t arg) {_register.a |= arg;       _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Or_A(uint16_t) {_register.a |= _register.a; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Or_B(uint16_t) {_register.a |= _register.b; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Or_C(uint16_t) {_register.a |= _register.c; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Or_D(uint16_t) {_register.a |= _register.d; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Or_E(uint16_t) {_register.a |= _register.e; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Or_H(uint16_t) {_register.a |= _register.h; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Or_L(uint16_t) {_register.a |= _register.l; _register.f = 0; _register.setFlagZ(_register.a == 0);}
		void Or_Hlp(uint16_t) {_register.a |= _memory[_register.hl]; _register.f = 0; _register.setFlagZ(_register.a == 0);}

		void cp_ref(uint16_t value) {
			_register.setFlagN(true);
			_register.setFlagZ(_register.a == value);
			_register.setFlagC(_register.a < value);
			_register.setFlagH((_register.a & 0xF) < (value & 0xF));
		}
		void Cp(uint16_t arg) {cp_ref(arg); }
		void Cp_A(uint16_t)   {cp_ref(_register.a);}
		void Cp_B(uint16_t)   {cp_ref(_register.b);}
		void Cp_C(uint16_t)   {cp_ref(_register.c);}
		void Cp_D(uint16_t)   {cp_ref(_register.d);}
		void Cp_E(uint16_t)   {cp_ref(_register.e);}
		void Cp_H(uint16_t)   {cp_ref(_register.h);}
		void Cp_L(uint16_t)   {cp_ref(_register.l);}
		void Cp_Hlp(uint16_t) {cp_ref(_memory[_register.hl]);}

		void Call(uint16_t arg)    {                            this->pushStack(_register.pc + 3); _register.pc = arg;}
		void Call_C(uint16_t arg)  {if (_register.getFlagC())  {this->pushStack(_register.pc + 3); _register.pc = arg;} else _register.m -= 3;}
		void Call_Z(uint16_t arg)  {if (_register.getFlagZ())  {this->pushStack(_register.pc + 3); _register.pc = arg;} else _register.m -= 3;}
		void Call_Nc(uint16_t arg) {if (!_register.getFlagC()) {this->pushStack(_register.pc + 3); _register.pc = arg;} else _register.m -= 3;}
		void Call_Nz(uint16_t arg) {if (!_register.getFlagZ()) {this->pushStack(_register.pc + 3); _register.pc = arg;} else _register.m -= 3;}

		void Push_De(uint16_t) {this->pushStack(_register.de);}
		void Push_Hl(uint16_t) {this->pushStack(_register.hl);}
		void Push_Bc(uint16_t) {this->pushStack(_register.bc);}
		void Push_Af(uint16_t) {this->pushStack(_register.af);}

		void Pop_De(uint16_t) {_register.de = this->popStack();}
		void Pop_Hl(uint16_t) {_register.hl = this->popStack();}
		void Pop_Af(uint16_t) {_register.af = this->popStack(); _register.f &= 0xF0;}
		void Pop_Bc(uint16_t) {_register.bc = this->popStack();}

		void Ret(uint16_t)    {_register.pc = this->popStack();}
		void Ret_Nz(uint16_t) {if (!_register.getFlagZ()) _register.pc = this->popStack(); else _register.m -= 3;}
		void Ret_Nc(uint16_t) {if (!_register.getFlagC()) _register.pc = this->popStack(); else _register.m -= 3;}
		void Ret_Z(uint16_t)  {if (_register.getFlagZ())  _register.pc = this->popStack(); else _register.m -= 3;}
		void Ret_C(uint16_t)  {if (_register.getFlagC())  _register.pc = this->popStack(); else _register.m -= 3;}

		void Jp(uint16_t arg)    {_register.pc = arg;}
		void Jp_Hlp(uint16_t)    {_register.pc = _memory[_register.hl];}
		void Jp_Nz(uint16_t arg) {if (!_register.getFlagZ()) _register.pc = arg; else _register.m -= 1;}
		void Jp_Nc(uint16_t arg) {if (!_register.getFlagC()) _register.pc = arg; else _register.m -= 1;}
		void Jp_Z(uint16_t arg)  {if (_register.getFlagZ())  _register.pc = arg; else _register.m -= 1;}
		void Jp_c(uint16_t arg)  {if (_register.getFlagC())  _register.pc = arg; else _register.m -= 1;}

		void Jr(uint16_t arg)    {_register.pc += (int8_t)arg + 2;}
		void Jr_Nz(uint16_t arg) {if (!_register.getFlagZ()) _register.pc += (int8_t)arg + 2; else _register.m -= 1;}
		void Jr_Nc(uint16_t arg) {if (!_register.getFlagC()) _register.pc += (int8_t)arg + 2; else _register.m -= 1;}
		void Jr_Z(uint16_t arg)  {if (_register.getFlagZ())  _register.pc += (int8_t)arg + 2; else _register.m -= 1;}
		void Jr_C(uint16_t arg)  {if (_register.getFlagC())  _register.pc += (int8_t)arg + 2; else _register.m -= 1;}

		void Ld_SP(uint16_t arg) {_memory[arg] = (_register.sp & 0b1111111100000000) >> 8; _memory[arg + 1] = _register.sp & 0b0000000011111111;}
		void Ld_BC(uint16_t arg) {_register.bc = arg;}
		void Ld_BC_A(uint16_t) {_memory[_register.bc] = _register.a;}
		void Ld_DE(uint16_t arg) {_register.de = arg;}
		void Ld_DE_A(uint16_t) {_memory[_register.de] = _register.a;}
		void Ld_Hlpp_A(uint16_t) {_memory[_register.hl] = _register.a; _register.hl++;}

		void Ld_HL_SP(uint16_t arg) {
			_register.setFlagH(uint8_t(_register.sp & 0xF) + (arg & 0xF) > 0xF);
			_register.setFlagC(false);
			_register.setFlagZ(false);
			_register.setFlagN(false);
			_register.hl = _register.sp + (int8_t)arg;
		}
		void Ld_Sp_Hl(uint16_t) {_register.sp = _register.hl;}
		void Ld_Cp(uint16_t) {_memory[0xFF00 + _register.c] = _register.a;}

		void Ld_Sp(uint16_t arg) {_register.sp = arg;}
		void Ld_Hl(uint16_t arg) { _register.hl = arg;}
		void Ld_Hln_A(uint16_t) {_memory[_register.hl] = _register.a; _register.hl--;}
		void Ld_Hlp(uint16_t arg) {_memory[_register.hl] = arg;}

		void Ldh_a_A(uint16_t arg) {_memory[0xFF00 + arg] = _register.a;}// arg + FF00 (I/O) ports)
		void Ldh_A_a(uint16_t arg) {_register.a = _memory[0xFF00 + arg];}// arg + FF00 (I/O) ports)

		void Ld_A_p(uint16_t arg) {_register.a = _memory[arg];}
		void Ld_a_A(uint16_t arg) {_memory[arg] = _register.a;}
		void Ld_A(uint16_t arg) {_register.a = arg;}
		void Ld_A_A(uint16_t) {_register.a = _register.a;}
		void Ld_A_B(uint16_t) {_register.a = _register.b;}
		void Ld_A_C(uint16_t) {_register.a = _register.c;}
		void Ld_A_D(uint16_t) {_register.a = _register.d;}
		void Ld_A_E(uint16_t) {_register.a = _register.e;}
		void Ld_A_H(uint16_t) {_register.a = _register.h;}
		void Ld_A_L(uint16_t) {_register.a = _register.l;}
		void Ld_A_Cp(uint16_t) {_register.a = _memory[_register.c + 0xFF00];} // c + FF00 (I/) ports)
		void Ld_A_BC(uint16_t) {_register.a = _memory[_register.bc];}
		void Ld_A_De(uint16_t) { _register.a = _memory[_register.de];}
		void Ld_A_Hln(uint16_t) {_register.a = _memory[_register.hl]; _register.hl--;}
		void Ld_A_Hlpp(uint16_t) {_register.a = _memory[_register.hl]; _register.hl++;}
		void Ld_A_Hlp(uint16_t) {std::cout << (int)_register.hl << " " <<  (int)_memory[_register.hl] << std::endl; _register.a = _memory[_register.hl];}

		void Ld_B(uint16_t arg) {_register.b = arg;};
		void Ld_B_B(uint16_t) {_register.b = _register.b;}
		void Ld_B_C(uint16_t) {_register.b = _register.c;}
		void Ld_B_D(uint16_t) {_register.b = _register.d;}
		void Ld_B_E(uint16_t) {_register.b = _register.e;}
		void Ld_B_H(uint16_t) {_register.b = _register.h;}
		void Ld_B_L(uint16_t) {_register.b = _register.l;}
		void Ld_B_Hlp(uint16_t) {_register.b = _memory[_register.hl];}
		void Ld_B_A(uint16_t) {_register.b = _register.a;}

		void Ld_C(uint16_t arg) {_register.c = arg;}
		void Ld_C_B(uint16_t) {_register.c = _register.b;}
		void Ld_C_C(uint16_t) {_register.c = _register.c;}
		void Ld_C_D(uint16_t) {_register.c = _register.d;}
		void Ld_C_E(uint16_t) {_register.c = _register.e;}
		void Ld_C_H(uint16_t) {_register.c = _register.h;}
		void Ld_C_L(uint16_t) {_register.c = _register.l;}
		void Ld_C_Hlp(uint16_t) {_register.c = _memory[_register.hl];}
		void Ld_C_A(uint16_t) {_register.c = _register.a;}

		void Ld_D(uint16_t arg) {_register.d = arg;}
		void Ld_D_B(uint16_t) {_register.d = _register.b;}
		void Ld_D_C(uint16_t) {_register.d = _register.c;}
		void Ld_D_D(uint16_t) {_register.d = _register.d;}
		void Ld_D_E(uint16_t) {_register.d = _register.e;}
		void Ld_D_H(uint16_t) {_register.d = _register.h;}
		void Ld_D_L(uint16_t) {_register.d = _register.l;}
		void Ld_D_Hlp(uint16_t) {_register.d = _memory[_register.hl];}
		void Ld_D_A(uint16_t) {_register.d = _register.a;}

		void Ld_E(uint16_t arg) {_register.e = arg;}
		void Ld_E_B(uint16_t) {_register.e = _register.b;}
		void Ld_E_C(uint16_t) {_register.e = _register.c;}
		void Ld_E_D(uint16_t) {_register.e = _register.d;}
		void Ld_E_E(uint16_t) {_register.e = _register.e;}
		void Ld_E_H(uint16_t) {_register.e = _register.h;}
		void Ld_E_L(uint16_t) {_register.e = _register.l;}
		void Ld_E_Hlp(uint16_t) {_register.e = _memory[_register.hl];}
		void Ld_E_A(uint16_t) {_register.e = _register.a;}

		void Ld_H(uint16_t arg) {_register.h = arg;}
		void Ld_H_B(uint16_t) {_register.h = _register.b;}
		void Ld_H_C(uint16_t) {_register.h = _register.c;}
		void Ld_H_D(uint16_t) {_register.h = _register.d;}
		void Ld_H_E(uint16_t) {_register.h = _register.e;}
		void Ld_H_H(uint16_t) {_register.h = _register.h;}
		void Ld_H_L(uint16_t) {_register.h = _register.l;}
		void Ld_H_Hlp(uint16_t) {_register.h = _memory[_register.hl];}
		void Ld_H_A(uint16_t) {_register.h = _register.a;}

		void Ld_L(uint16_t arg) {_register.l = arg;}
		void Ld_L_B(uint16_t)   {_register.l = _register.b;}
		void Ld_L_C(uint16_t)   {_register.l = _register.c;}
		void Ld_L_D(uint16_t)   {_register.l = _register.d;}
		void Ld_L_E(uint16_t)   {_register.l = _register.e;}
		void Ld_L_H(uint16_t)   {_register.l = _register.h;}
		void Ld_L_L(uint16_t)   {_register.l = _register.l;}
		void Ld_L_Hlp(uint16_t) {_register.l = _memory[_register.hl];}
		void Ld_L_A(uint16_t)   {_register.l = _register.a;}

		void Ld_Hlp_B(uint16_t) {_memory[_register.hl] = _register.b;}
		void Ld_Hlp_C(uint16_t) {_memory[_register.hl] = _register.c;}
		void Ld_Hlp_D(uint16_t) {_memory[_register.hl] = _register.d;}
		void Ld_Hlp_E(uint16_t) {_memory[_register.hl] = _register.e;}
		void Ld_Hlp_H(uint16_t) {_memory[_register.hl] = _register.h;}
		void Ld_Hlp_L(uint16_t) {_memory[_register.hl] = _register.l;}
		void Ld_Hlp_A(uint16_t) {_memory[_register.hl] = _register.a;}

		void RLC_B()   {rlc(_register.b); _register.setFlagZ(_register.b == 0);}
		void RLC_C()   {rlc(_register.c); _register.setFlagZ(_register.c == 0);}
		void RLC_D()   {rlc(_register.d); _register.setFlagZ(_register.d == 0);}
		void RLC_E()   {rlc(_register.e); _register.setFlagZ(_register.e == 0);}
		void RLC_H()   {rlc(_register.h); _register.setFlagZ(_register.h == 0);}
		void RLC_L()   {rlc(_register.l); _register.setFlagZ(_register.l == 0);}
		void RLC_HLp() {rlc(_memory[_register.hl]); _register.setFlagZ(_memory[_register.hl] == 0);}
		void RLC_A()   {rlc(_register.a); _register.setFlagZ(_register.a == 0);}

		void RRC_B()   {rrc(_register.b); _register.setFlagZ(_register.b == 0);}
		void RRC_C()   {rrc(_register.c); _register.setFlagZ(_register.c == 0);}
		void RRC_D()   {rrc(_register.d); _register.setFlagZ(_register.d == 0);}
		void RRC_E()   {rrc(_register.e); _register.setFlagZ(_register.e == 0);}
		void RRC_H()   {rrc(_register.h); _register.setFlagZ(_register.h == 0);}
		void RRC_L()   {rrc(_register.l); _register.setFlagZ(_register.l == 0);}
		void RRC_HLp() {rrc(_memory[_register.hl]); _register.setFlagZ(_memory[_register.hl] == 0);}
		void RRC_A()   {rrc(_register.a); _register.setFlagZ(_register.a == 0);}

		void RL_B()   {rl(_register.b);           _register.setFlagZ(_register.b == 0);}
		void RL_C()   {rl(_register.c);           _register.setFlagZ(_register.c == 0);}
		void RL_D()   {rl(_register.d);           _register.setFlagZ(_register.d == 0);}
		void RL_E()   {rl(_register.e);           _register.setFlagZ(_register.e == 0);}
		void RL_H()   {rl(_register.h);           _register.setFlagZ(_register.h == 0);}
		void RL_L()   {rl(_register.l);           _register.setFlagZ(_register.l == 0);}
		void RL_HLp() {rl(_memory[_register.hl]); _register.setFlagZ(_memory[_register.hl] == 0);}
		void RL_A()   {rl(_register.a);           _register.setFlagZ(_register.a == 0);}

		void RR_B()   {rr(_register.b);           _register.setFlagZ(_register.b == 0);}
		void RR_C()   {rr(_register.c);           _register.setFlagZ(_register.c == 0);}
		void RR_D()   {rr(_register.d);           _register.setFlagZ(_register.d == 0);}
		void RR_E()   {rr(_register.e);           _register.setFlagZ(_register.e == 0);}
		void RR_H()   {rr(_register.h);           _register.setFlagZ(_register.h == 0);}
		void RR_L()   {rr(_register.l);           _register.setFlagZ(_register.l == 0);}
		void RR_HLp() {rr(_memory[_register.hl]); _register.setFlagZ(_memory[_register.hl] == 0);}
		void RR_A()   {rr(_register.a);           _register.setFlagZ(_register.a == 0);}

		void SLA_B()   {l(_register.b);           _register.setFlagZ(_register.b == 0);};
		void SLA_C()   {l(_register.c);           _register.setFlagZ(_register.c == 0);};
		void SLA_D()   {l(_register.d);           _register.setFlagZ(_register.d == 0);};
		void SLA_E()   {l(_register.e);           _register.setFlagZ(_register.e == 0);};
		void SLA_H()   {l(_register.h);           _register.setFlagZ(_register.h == 0);};
		void SLA_L()   {l(_register.l);           _register.setFlagZ(_register.l == 0);};
		void SLA_HLp() {l(_memory[_register.hl]); _register.setFlagZ(_memory[_register.hl] == 0);}
		void SLA_A()   {l(_register.a);           _register.setFlagZ(_register.a == 0);};

		void SRA_B()   {r(_register.b);           _register.b |= ((_register.b & 0b01000000) << 1);                     _register.setFlagZ(_register.b == 0);};
		void SRA_C()   {r(_register.c);           _register.c |= ((_register.c & 0b01000000) << 1);                     _register.setFlagZ(_register.c == 0);};
		void SRA_D()   {r(_register.d);           _register.d |= ((_register.d & 0b01000000) << 1);                     _register.setFlagZ(_register.d == 0);};
		void SRA_E()   {r(_register.e);           _register.e |= ((_register.e & 0b01000000) << 1);                     _register.setFlagZ(_register.e == 0);};
		void SRA_H()   {r(_register.h);           _register.h |= ((_register.h & 0b01000000) << 1);                     _register.setFlagZ(_register.h == 0);};
		void SRA_L()   {r(_register.l);           _register.l |= ((_register.l & 0b01000000) << 1);                     _register.setFlagZ(_register.l == 0);};
		void SRA_HLp() {r(_memory[_register.hl]); _memory[_register.hl] |= ((_memory[_register.hl] & 0b01000000) << 1); _register.setFlagZ(_memory[_register.hl] == 0);};
		void SRA_A()   {r(_register.a);           _register.a |= ((_register.a & 0b01000000) << 1);                     _register.setFlagZ(_register.a == 0);};

		void SWAP_B()   {auto ci = (0b1111 & _register.b) << 4; _register.b = (_register.b >> 4) + ci; _register.f = 0; _register.setFlagZ(_register.b == 0);}
		void SWAP_C()   {auto ci = (0b1111 & _register.c) << 4; _register.c = (_register.c >> 4) + ci; _register.f = 0; _register.setFlagZ(_register.c == 0);}
		void SWAP_D()   {auto ci = (0b1111 & _register.d) << 4; _register.d = (_register.d >> 4) + ci; _register.f = 0; _register.setFlagZ(_register.d == 0);}
		void SWAP_E()   {auto ci = (0b1111 & _register.e) << 4; _register.e = (_register.e >> 4) + ci; _register.f = 0; _register.setFlagZ(_register.e == 0);}
		void SWAP_H()   {auto ci = (0b1111 & _register.h) << 4; _register.h = (_register.h >> 4) + ci; _register.f = 0; _register.setFlagZ(_register.h == 0);}
		void SWAP_L()   {auto ci = (0b1111 & _register.l) << 4; _register.l = (_register.l >> 4) + ci; _register.f = 0; _register.setFlagZ(_register.l == 0);}
		void SWAP_HLp() {auto ci = (0b1111 & _memory[_register.hl]) << 4; _memory[_register.hl] = (_memory[_register.hl] >> 4) + ci; _register.f = 0; _register.setFlagZ(_memory[_register.hl] == 0);}
		void SWAP_A()   {auto ci = (0b1111 & _register.a) << 4; _register.a = (_register.a >> 4) + ci; _register.f = 0; _register.setFlagZ(_register.a == 0);}

		void SRL_B()   {r(_register.b);           _register.setFlagZ(_register.b == 0);}
		void SRL_C()   {r(_register.c);           _register.setFlagZ(_register.c == 0);}
		void SRL_D()   {r(_register.d);           _register.setFlagZ(_register.d == 0);}
		void SRL_E()   {r(_register.e);           _register.setFlagZ(_register.e == 0);}
		void SRL_H()   {r(_register.h);           _register.setFlagZ(_register.h == 0);}
		void SRL_L()   {r(_register.l);           _register.setFlagZ(_register.l == 0);}
		void SRL_HLp() {r(_memory[_register.hl]); _register.setFlagZ(_memory[_register.hl] == 0);}
		void SRL_A()   {r(_register.a);           _register.setFlagZ(_register.a == 0);}

		void BIT_0_B() {_register.setFlagZ((_register.b & 0b1) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_0_C() {_register.setFlagZ((_register.c & 0b1) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_0_D() {_register.setFlagZ((_register.d & 0b1) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_0_E() {_register.setFlagZ((_register.e & 0b1) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_0_H() {_register.setFlagZ((_register.h & 0b1) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_0_L() {_register.setFlagZ((_register.l & 0b1) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_0_Hlp() {_register.setFlagZ((_memory[_register.hl] & 0b1) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_0_A() {_register.setFlagZ((_register.a & 0b1) == 0); _register.setFlagN(false); _register.setFlagH(true);}

		void BIT_1_B() {_register.setFlagZ((_register.b & 0b10) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_1_C() {_register.setFlagZ((_register.c & 0b10) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_1_D() {_register.setFlagZ((_register.d & 0b10) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_1_E() {_register.setFlagZ((_register.e & 0b10) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_1_H() {_register.setFlagZ((_register.h & 0b10) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_1_L() {_register.setFlagZ((_register.l & 0b10) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_1_Hlp() {_register.setFlagZ((_memory[_register.hl] & 0b10) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_1_A() {_register.setFlagZ((_register.a & 0b10) == 0); _register.setFlagN(false); _register.setFlagH(true);}

		void BIT_2_B() {_register.setFlagZ((_register.b & 0b100) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_2_C() {_register.setFlagZ((_register.c & 0b100) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_2_D() {_register.setFlagZ((_register.d & 0b100) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_2_E() {_register.setFlagZ((_register.e & 0b100) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_2_H() {_register.setFlagZ((_register.h & 0b100) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_2_L() {_register.setFlagZ((_register.l & 0b100) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_2_Hlp() {_register.setFlagZ((_memory[_register.hl] & 0b100) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_2_A() {_register.setFlagZ((_register.a & 0b100) == 0); _register.setFlagN(false); _register.setFlagH(true);}

		void BIT_3_B() {_register.setFlagZ((_register.b & 0b1000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_3_C() {_register.setFlagZ((_register.c & 0b1000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_3_D() {_register.setFlagZ((_register.d & 0b1000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_3_E() {_register.setFlagZ((_register.e & 0b1000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_3_H() {_register.setFlagZ((_register.h & 0b1000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_3_L() {_register.setFlagZ((_register.l & 0b1000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_3_Hlp() {_register.setFlagZ((_memory[_register.hl] & 0b1000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_3_A() {_register.setFlagZ((_register.a & 0b1000) == 0); _register.setFlagN(false); _register.setFlagH(true);}

		void BIT_4_B() {_register.setFlagZ((_register.b & 0b10000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_4_C() {_register.setFlagZ((_register.c & 0b10000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_4_D() {_register.setFlagZ((_register.d & 0b10000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_4_E() {_register.setFlagZ((_register.e & 0b10000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_4_H() {_register.setFlagZ((_register.h & 0b10000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_4_L() {_register.setFlagZ((_register.l & 0b10000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_4_Hlp() {_register.setFlagZ((_memory[_register.hl] & 0b10000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_4_A() {_register.setFlagZ((_register.a & 0b10000) == 0); _register.setFlagN(false); _register.setFlagH(true);}

		void BIT_5_B() {_register.setFlagZ((_register.b & 0b100000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_5_C() {_register.setFlagZ((_register.c & 0b100000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_5_D() {_register.setFlagZ((_register.d & 0b100000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_5_E() {_register.setFlagZ((_register.e & 0b100000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_5_H() {_register.setFlagZ((_register.h & 0b100000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_5_L() {_register.setFlagZ((_register.l & 0b100000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_5_Hlp() {_register.setFlagZ((_memory[_register.hl] & 0b100000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_5_A() {_register.setFlagZ((_register.a & 0b100000) == 0); _register.setFlagN(false); _register.setFlagH(true);}

		void BIT_6_B() {_register.setFlagZ((_register.b & 0b1000000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_6_C() {_register.setFlagZ((_register.c & 0b1000000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_6_D() {_register.setFlagZ((_register.d & 0b1000000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_6_E() {_register.setFlagZ((_register.e & 0b1000000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_6_H() {_register.setFlagZ((_register.h & 0b1000000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_6_L() {_register.setFlagZ((_register.l & 0b1000000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_6_Hlp() {_register.setFlagZ((_memory[_register.hl] & 0b1000000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_6_A() {_register.setFlagZ((_register.a & 0b1000000) == 0); _register.setFlagN(false); _register.setFlagH(true);}

		void BIT_7_B() {_register.setFlagZ((_register.b & 0b10000000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_7_C() {_register.setFlagZ((_register.c & 0b10000000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_7_D() {_register.setFlagZ((_register.d & 0b10000000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_7_E() {_register.setFlagZ((_register.e & 0b10000000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_7_H() {_register.setFlagZ((_register.h & 0b10000000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_7_L() {_register.setFlagZ((_register.l & 0b10000000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_7_Hlp() {_register.setFlagZ((_memory[_register.hl] & 0b10000000) == 0); _register.setFlagN(false); _register.setFlagH(true);}
		void BIT_7_A() {_register.setFlagZ((_register.a & 0b10000000) == 0); _register.setFlagN(false); _register.setFlagH(true);}

		void RES_0_B() {_register.b &= 0b11111110;}
		void RES_0_C() {_register.c &= 0b11111110;}
		void RES_0_D() {_register.d &= 0b11111110;}
		void RES_0_E() {_register.e &= 0b11111110;}
		void RES_0_H() {_register.h &= 0b11111110;}
		void RES_0_L() {_register.l &= 0b11111110;}
		void RES_0_Hlp() {_memory[_register.hl] &= 0b11111110;}
		void RES_0_A() {_register.a &= 0b11111110;}

		void RES_1_B() {_register.b &= 0b11111101;}
		void RES_1_C() {_register.c &= 0b11111101;}
		void RES_1_D() {_register.d &= 0b11111101;}
		void RES_1_E() {_register.e &= 0b11111101;}
		void RES_1_H() {_register.h &= 0b11111101;}
		void RES_1_L() {_register.l &= 0b11111101;}
		void RES_1_Hlp() {_memory[_register.hl] &= 0b11111101;};
		void RES_1_A() {_register.a &= 0b11111101;};

		void RES_2_B() {_register.b &= 0b11111011;};
		void RES_2_C() {_register.c &= 0b11111011;};
		void RES_2_D() {_register.d &= 0b11111011;};
		void RES_2_E() {_register.e &= 0b11111011;};
		void RES_2_H() {_register.h &= 0b11111011;};
		void RES_2_L() {_register.l &= 0b11111011;};
		void RES_2_Hlp() {_memory[_register.hl] &= 0b11111011;};
		void RES_2_A() {_register.a &= 0b11111011;};

		void RES_3_B() {_register.b &= 0b11110111;};
		void RES_3_C() {_register.c &= 0b11110111;};
		void RES_3_D() {_register.d &= 0b11110111;};
		void RES_3_E() {_register.e &= 0b11110111;};
		void RES_3_H() {_register.h &= 0b11110111;};
		void RES_3_L() {_register.l &= 0b11110111;};
		void RES_3_Hlp() {_memory[_register.hl] &= 0b11110111;};
		void RES_3_A() {_register.a &= 0b11110111;};

		void RES_4_B() {_register.b &= 0b11101111;};
		void RES_4_C() {_register.c &= 0b11101111;};
		void RES_4_D() {_register.d &= 0b11101111;};
		void RES_4_E() {_register.e &= 0b11101111;};
		void RES_4_H() {_register.h &= 0b11101111;};
		void RES_4_L() {_register.l &= 0b11101111;};
		void RES_4_Hlp() {_memory[_register.hl] &= 0b11101111;};
		void RES_4_A() {_register.a &= 0b11101111;};

		void RES_5_B() {_register.b &= 0b11011111;};
		void RES_5_C() {_register.c &= 0b11011111;};
		void RES_5_D() {_register.d &= 0b11011111;};
		void RES_5_E() {_register.e &= 0b11011111;};
		void RES_5_H() {_register.h &= 0b11011111;};
		void RES_5_L() {_register.l &= 0b11011111;};
		void RES_5_Hlp() {_memory[_register.hl] &= 0b11011111;};
		void RES_5_A() {_register.a &= 0b11011111;};

		void RES_6_B() {_register.b &= 0b10111111;};
		void RES_6_C() {_register.c &= 0b10111111;};
		void RES_6_D() {_register.d &= 0b10111111;};
		void RES_6_E() {_register.e &= 0b10111111;};
		void RES_6_H() {_register.h &= 0b10111111;};
		void RES_6_L() {_register.l &= 0b10111111;};
		void RES_6_Hlp() {_memory[_register.hl] &= 0b10111111;};
		void RES_6_A() {_register.a &= 0b10111111;};

		void RES_7_B() {_register.b &= 0b01111111;};
		void RES_7_C() {_register.c &= 0b01111111;};
		void RES_7_D() {_register.d &= 0b01111111;};
		void RES_7_E() {_register.e &= 0b01111111;};
		void RES_7_H() {_register.h &= 0b01111111;};
		void RES_7_L() {_register.l &= 0b01111111;};
		void RES_7_Hlp() {_memory[_register.hl] &= 0b01111111;};
		void RES_7_A() {_register.a &= 0b01111111;};

		void SET_0_B() {_register.b |= 0b1;};
		void SET_0_C() {_register.a |= 0b1;};
		void SET_0_D() {_register.c |= 0b1;};
		void SET_0_E() {_register.d |= 0b1;};
		void SET_0_H() {_register.e |= 0b1;};
		void SET_0_L() {_register.h |= 0b1;};
		void SET_0_Hlp() {_memory[_register.hl] |= 0b1;};
		void SET_0_A() {_register.a |= 0b1;};

		void SET_1_B() {_register.b |= 0b10;};
		void SET_1_C() {_register.c |= 0b10;};
		void SET_1_D() {_register.d |= 0b10;};
		void SET_1_E() {_register.e |= 0b10;};
		void SET_1_H() {_register.h |= 0b10;};
		void SET_1_L() {_register.l |= 0b10;};
		void SET_1_Hlp() {_memory[_register.hl] |= 0b10;};
		void SET_1_A() {_register.a |= 0b10;};

		void SET_2_B() {_register.b |= 0b100;};
		void SET_2_C() {_register.c |= 0b100;};
		void SET_2_D() {_register.d |= 0b100;};
		void SET_2_E() {_register.e |= 0b100;};
		void SET_2_H() {_register.h |= 0b100;};
		void SET_2_L() {_register.l |= 0b100;};
		void SET_2_Hlp() {_memory[_register.hl] |= 0b100;};
		void SET_2_A() {_register.a |= 0b100;};

		void SET_3_B() {_register.b |= 0b1000;};
		void SET_3_C() {_register.c |= 0b1000;};
		void SET_3_D() {_register.d |= 0b1000;};
		void SET_3_E() {_register.e |= 0b1000;};
		void SET_3_H() {_register.h |= 0b1000;};
		void SET_3_L() {_register.l |= 0b1000;};
		void SET_3_Hlp() {_memory[_register.hl] |= 0b1000;};
		void SET_3_A() {_register.a |= 0b1000;};

		void SET_4_B() {_register.b |= 0b10000;};
		void SET_4_C() {_register.c |= 0b10000;};
		void SET_4_D() {_register.d |= 0b10000;};
		void SET_4_E() {_register.e |= 0b10000;};
		void SET_4_H() {_register.h |= 0b10000;};
		void SET_4_L() {_register.l |= 0b10000;};
		void SET_4_Hlp() {_memory[_register.hl] |= 0b10000;};
		void SET_4_A() {_register.a |= 0b10000;};

		void SET_5_B() {_register.b |= 0b100000;};
		void SET_5_C() {_register.c |= 0b100000;};
		void SET_5_D() {_register.d |= 0b100000;};
		void SET_5_E() {_register.e |= 0b100000;};
		void SET_5_H() {_register.h |= 0b100000;};
		void SET_5_L() {_register.l |= 0b100000;};
		void SET_5_Hlp() {_memory[_register.hl] |= 0b100000;};
		void SET_5_A() {_register.a |= 0b100000;};

		void SET_6_B() {_register.b |= 0b1000000;};
		void SET_6_C() {_register.c |= 0b1000000;};
		void SET_6_D() {_register.d |= 0b1000000;};
		void SET_6_E() {_register.e |= 0b1000000;};
		void SET_6_H() {_register.h |= 0b1000000;};
		void SET_6_L() {_register.l |= 0b1000000;};
		void SET_6_Hlp() {_memory[_register.hl] |= 0b1000000;};
		void SET_6_A() {_register.a |= 0b1000000;};

		void SET_7_B() {_register.b |= 0b10000000;};
		void SET_7_C() {_register.c |= 0b10000000;};
		void SET_7_D() {_register.d |= 0b10000000;};
		void SET_7_E() {_register.e |= 0b10000000;};
		void SET_7_H() {_register.h |= 0b10000000;};
		void SET_7_L() {_register.l |= 0b10000000;};
		void SET_7_Hlp() {_memory[_register.hl] |= 0b10000000;};
		void SET_7_A() {_register.a |= 0b10000000;};

		void default_operator(uint16_t) {std::cout << "unknown operand: " << managedInstruction[_memory[_register.pc]]._name << std::endl; exit(1);}
	};
}

std::ostream &operator<<(std::ostream &, const emulator::Cpu::instructionInfos &);

#endif //EMULATOR_GAMEBOY_CPU_HPP
