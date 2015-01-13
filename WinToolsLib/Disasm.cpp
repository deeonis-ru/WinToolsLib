#include "Disasm.h"
#include "Exception.h"
#include "libudis86\extern.h"
#include <map>

namespace WinToolsLib
{
	using namespace Details;

	Disasm::Disasm(IStream* stream, UInt8 bits) :
		m_hookDestination(0),
		m_hookSize(0)
	{
		ud_init(&m_ud);
		ud_set_mode(&m_ud, bits);

		m_ud.inp_buff = (uint8_t*)stream;
		m_ud.inp_hook = InputHook;
	}

	Bool Disasm::IsHook(UInt64 va, SizeT size)
	{
		ud_set_pc(&m_ud, va);
		
		m_hookDestination = 0;
		m_hookSize = 0;

		Int64 push = 0;
		std::map<Int32, Int64> regs;

		Bool isHook = False;
		while (True)
		{
			auto bytes = ud_disassemble(&m_ud);
			if (0 == bytes)
			{
				break;
			}

			m_hookSize += bytes;

			if (UD_Ijmp == m_ud.mnemonic)
			{
				if (UD_OP_JIMM == m_ud.operand[0].type)
				{
					m_hookDestination = (UInt64)((Int64)m_ud.pc + GetOperandValue(&m_ud, 0));
					isHook = True;
					break;
				}
				
				if (UD_OP_MEM == m_ud.operand[0].type)
				{
					isHook = ReadMemoryOperand(&m_ud, &m_hookDestination);
					break;
				}

				if (UD_OP_REG == m_ud.operand[0].type)
				{
					auto found = regs.find(m_ud.operand[0].base);
					if (found != regs.end())
					{
						m_hookDestination = found->second;
						isHook = True;
						break;
					}
				}

				break;
			}

			if (UD_Icall == m_ud.mnemonic)
			{
				if (UD_OP_JIMM == m_ud.operand[0].type)
				{
					m_hookDestination = (UInt64)((Int64)m_ud.pc + GetOperandValue(&m_ud, 0));
					isHook = True;
					break;
				}

				if (UD_OP_MEM == m_ud.operand[0].type)
				{
					isHook = ReadMemoryOperand(&m_ud, &m_hookDestination);
					break;
				}

				break;
			}

			if (UD_Ipush == m_ud.mnemonic && UD_OP_IMM == m_ud.operand[0].type)
			{
				push = GetOperandValue(&m_ud, 0);
				continue;
			}

			if (UD_Iret == m_ud.mnemonic && push)
			{
				m_hookDestination = push;
				isHook = True;
				break;
			}

			if (UD_Imov == m_ud.mnemonic && UD_OP_REG == m_ud.operand[0].type)
			{
				regs[m_ud.operand[0].base] = GetOperandValue(&m_ud, 1);
				continue;
			}

			if (UD_Inop == m_ud.mnemonic)
			{
				continue;
			}

			break;
		}

		if (isHook)
		{
			while (m_hookSize < size)
			{
				auto bytes = ud_disassemble(&m_ud);
				if (0 == bytes)
				{
					break;
				}

				if (UD_Iint3 == m_ud.mnemonic)
				{
					m_hookSize += bytes;
					continue;
				}

				if (UD_Inop == m_ud.mnemonic)
				{
					m_hookSize += bytes;
					continue;
				}

				break;
			}
		}

		return isHook;
	}

	Bool Disasm::IsTrampoline(UInt64 va, SizeT size)
	{
		ud_set_pc(&m_ud, va);
		
		m_hookDestination = 0;
		m_hookSize = 0;

		std::map<UInt64, SizeT> destinations;

		while (True)
		{
			auto bytes = ud_disassemble(&m_ud);
			if (0 == bytes)
			{
				break;
			}

			m_hookSize += bytes;
			UInt64 pc = m_ud.pc + 1;

			auto found = destinations.find(pc - bytes);
			if (found != destinations.end())
			{
				destinations.erase(found);
			}

			if (UD_Ijo <= m_ud.mnemonic &&
				UD_Ijmp >= m_ud.mnemonic)
			{
				if (UD_OP_JIMM == m_ud.operand[0].type)
				{
					auto dest = (UInt64)((Int64)pc + GetOperandValue(&m_ud, 0));

					auto found = destinations.find(dest);
					if (found != destinations.end())
					{
						found->second++;
					}
					else
					{
						destinations[dest] = 1;
					}
				}
				else if (UD_OP_MEM == m_ud.operand[0].type)
				{
					UInt64 dest = 0;

					auto success = ReadMemoryOperand(&m_ud, &dest);
					if (!success)
					{
						break;
					}
					
					auto found = destinations.find(dest);
					if (found != destinations.end())
					{
						found->second++;
					}
					else
					{
						destinations[dest] = 1;
					}

					if (64 == m_ud.dis_mode &&
						0 == GetOffsetValue(&m_ud, 0))
					{
						Skip(&m_ud, 8);
					}
				}
				else
				{
					break;
				}
			}

			if (UD_Iret == m_ud.mnemonic)
			{
				if (destinations.size() == 1)
				{
					m_hookDestination = destinations.begin()->first;
					return True;
				}
				break;
			}
		}

		return False;
	}
	
	Int32 Disasm::InputHook(ud_t* ud)
	{
		try
		{
			Byte byte;
			
			IStream* stream = (IStream*)ud->inp_buff;
			stream->Read(&byte, sizeof(byte));

			return byte;
		}
		catch (Exception&)
		{
			return -1;
		}
	}

	Int64 Disasm::GetOperandValue(ud_t* ud, SizeT i)
	{
		if (8 == ud->operand[i].size)
		{
			return ud->operand[i].lval.sbyte;
		}
		if (16 == ud->operand[i].size)
		{
			return ud->operand[i].lval.sword;
		}
		if (32 == ud->operand[i].size)
		{
			return ud->operand[i].lval.sdword;
		}
		return ud->operand[i].lval.sqword;
	}

	Int64 Disasm::GetOffsetValue(ud_t* ud, SizeT i)
	{
		if (8 == ud->operand[i].offset)
		{
			return ud->operand[i].lval.sbyte;
		}
		if (16 == ud->operand[i].offset)
		{
			return ud->operand[i].lval.sword;
		}
		if (32 == ud->operand[i].offset)
		{
			return ud->operand[i].lval.sdword;
		}
		return ud->operand[i].lval.sqword;
	}

	Bool Disasm::ReadMemoryOperand(Details::ud_t* ud, UInt64* value)
	{
		try
		{
			auto dest = GetOffsetValue(ud, 0);
			auto offset = (32 == ud->dis_mode) ? dest - (Int64)ud->pc : dest;

			auto stream = (IStream*)ud->inp_buff;
			auto curpos = (Int64)stream->GetPosition();
			auto newpos = curpos + offset;
					
			stream->SetPosition((SizeT)newpos);
			stream->Read((PByte)value, ud->operand[0].size / 8);
			stream->SetPosition((SizeT)curpos);

			return True;
		}
		catch (...)
		{
			return False;
		}
	}

	Void Disasm::Skip(Details::ud_t* ud, SizeT bytes)
	{
		auto stream = (IStream*)ud->inp_buff;
		stream->Skip(bytes);
		ud->pc += bytes;
	}
}
