#pragma once
#include "..\Types.h"
#include "HandleBase.h"
#include <Windows.h>

namespace WinToolsLib
{
	namespace Handles
	{
		class RegistryHandle : public HandleBase<HKEY>
		{
			typedef HandleBase<HKEY> Base;

		public:
			RegistryHandle();
			RegistryHandle(HKEY handle);
			RegistryHandle(RegistryHandle&& other);

			RegistryHandle& operator=(RegistryHandle&& other);

		protected:
			Void MoveFrom(RegistryHandle&& other);
			Void CopyFrom(const RegistryHandle& other);

			static Void Finalizer(HKEY handle);
		};
	}
}