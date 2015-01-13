#pragma once
#include "..\Types.h"
#include "HandleBase.h"
#include <Windows.h>

namespace WinToolsLib
{
	namespace Handles
	{
		class Handle : public HandleBase<HANDLE>
		{
			typedef HandleBase<HANDLE> Base;

		public:
			Handle();
			Handle(HANDLE handle);
			Handle(Handle&& other);
			Handle(const Handle& other);

			Bool IsValid() const;
			Void Duplicate(HANDLE handle);

			Handle& operator=(Handle&& other);
			Handle& operator=(const Handle& other);

		protected:
			Void MoveFrom(Handle&& other);
			Void CopyFrom(const Handle& other);

			static Void Finalizer(HANDLE handle);
		};
	}
}