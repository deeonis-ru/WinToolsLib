#include "RegistryHandle.h"

namespace WinToolsLib
{
	namespace Handles
	{
		RegistryHandle::RegistryHandle() :
			Base(RegistryHandle::Finalizer)
		{
		}

		RegistryHandle::RegistryHandle(HKEY handle) :
			Base(RegistryHandle::Finalizer, handle)
		{
		}

		RegistryHandle::RegistryHandle(RegistryHandle&& other) :
			Base(RegistryHandle::Finalizer)
		{
			MoveFrom(std::move(other));
		}

		RegistryHandle& RegistryHandle::operator=(RegistryHandle&& other)
		{
			MoveFrom(std::move(other));
			return *this;
		}

		Void RegistryHandle::MoveFrom(RegistryHandle&& other)
		{
			if (this != &other)
			{
				Reset(other.m_handle);
				other.m_handle = NULL;
			}
		}

		Void RegistryHandle::Finalizer(HKEY handle)
		{
			::RegCloseKey(handle);
		}
	}
}