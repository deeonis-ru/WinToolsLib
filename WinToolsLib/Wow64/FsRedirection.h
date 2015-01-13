#pragma once
#include "..\Types.h"

namespace WinToolsLib
{
	namespace Wow64
	{
		class FsRedirection
		{
			FsRedirection(const FsRedirection& other);            // Non-copyable
			FsRedirection& operator=(const FsRedirection& other); // Non-copyable

		public:
			FsRedirection();
			~FsRedirection();

			FsRedirection(FsRedirection&& other);
			FsRedirection& operator=(FsRedirection&& other);

			Void Disable();
			Void Revert();

		private:
			Bool  m_shouldRevert;
			PVoid m_oldValue;
		};
	}
}
