#pragma once
#include "..\Types.h"
#include "HandleBase.h"
#include <Windows.h>
#include <Wininet.h>

namespace WinToolsLib
{
	namespace Handles
	{
		class InternetHandle : public HandleBase<HINTERNET>
		{
			typedef HandleBase<HINTERNET> Base;

		public:
			InternetHandle();
			InternetHandle(HINTERNET handle);
			InternetHandle(InternetHandle&& other);

			InternetHandle& operator=(InternetHandle&& other);

		protected:
			Void MoveFrom(InternetHandle&& other);
			Void CopyFrom(const InternetHandle& other);

			static Void Finalizer(HINTERNET handle);
		};
	}
}