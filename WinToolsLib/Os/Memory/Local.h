#pragma once
#include "..\..\Types.h"

#include <Windows.h>

namespace WinToolsLib
{
	namespace Memory
	{
		class Local
		{
		public:
			Local(HLOCAL hLocal);
			~Local();

			Void Free();

			PVoid Lock();
			Void Unlock();

			static Local Alloc(UInt32 flags, SizeT bytes);

		private:
			HLOCAL m_hLocal;
			PVoid  m_pointer;
		};
	}
}
