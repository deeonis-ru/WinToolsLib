#pragma once
#include "..\Types.h"
#include "HandleBase.h"
#include <Windows.h>

namespace WinToolsLib
{
	namespace Handles
	{
		class ServiceHandle : public HandleBase<SC_HANDLE>
		{
			typedef HandleBase<SC_HANDLE> Base;

		public:
			ServiceHandle();
			ServiceHandle(SC_HANDLE handle);
			ServiceHandle(ServiceHandle&& other);

			ServiceHandle& operator=(ServiceHandle&& other);

		protected:
			Void MoveFrom(ServiceHandle&& other);
			Void CopyFrom(const ServiceHandle& other);

			static Void Finalizer(SC_HANDLE handle);
		};
	}
}