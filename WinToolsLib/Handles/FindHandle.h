#pragma once
#include "..\Types.h"
#include "HandleBase.h"
#include <Windows.h>

namespace WinToolsLib
{
	namespace Handles
	{
		class FindHandle : public HandleBase<HANDLE>
		{
			typedef HandleBase<HANDLE> Base;

		public:
			FindHandle();
			FindHandle(HANDLE handle);
			FindHandle(FindHandle&& other);

			Bool IsValid() const;

			FindHandle& operator=(FindHandle&& other);

		protected:
			Void MoveFrom(FindHandle&& other);
			Void CopyFrom(const FindHandle& other);

			static Void Finalizer(HANDLE handle);
		};
	}
}