#pragma once
#include "Types.h"
#include "IStream.h"

namespace WinToolsLib
{
	namespace Details
	{
		#include "libudis86\extern.h"
	}

	class Disasm
	{
	public:
		Disasm(IStream* stream, UInt8 bits);

		Bool IsHook(UInt64 va, SizeT size);
		Bool IsTrampoline(UInt64 va, SizeT size);
		
		UInt64 GetHookDestination() const;
		SizeT GetHookSize() const;

	private:
		static Int32 InputHook(Details::ud_t* ud);
		
		static Int64 GetOperandValue(Details::ud_t* ud, SizeT i);
		static Int64 GetOffsetValue(Details::ud_t* ud, SizeT i);

		static Bool ReadMemoryOperand(Details::ud_t* ud, UInt64* value);
		static Void Skip(Details::ud_t* ud, SizeT bytes);

	private:
		Details::ud_t m_ud;
		UInt64        m_hookDestination;
		SizeT         m_hookSize;
	};

	inline UInt64 Disasm::GetHookDestination() const
	{
		return m_hookDestination;
	}
	
	inline SizeT Disasm::GetHookSize() const
	{
		return m_hookSize;
	}
}
