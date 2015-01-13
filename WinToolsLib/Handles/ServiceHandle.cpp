#include "ServiceHandle.h"

namespace WinToolsLib
{
	namespace Handles
	{
		ServiceHandle::ServiceHandle() :
			Base(ServiceHandle::Finalizer)
		{
		}

		ServiceHandle::ServiceHandle(SC_HANDLE handle) :
			Base(ServiceHandle::Finalizer, handle)
		{
		}

		ServiceHandle::ServiceHandle(ServiceHandle&& other) :
			Base(ServiceHandle::Finalizer)
		{
			MoveFrom(std::move(other));
		}

		ServiceHandle& ServiceHandle::operator=(ServiceHandle&& other)
		{
			MoveFrom(std::move(other));
			return *this;
		}

		Void ServiceHandle::MoveFrom(ServiceHandle&& other)
		{
			if (this != &other)
			{
				Reset(other.m_handle);
				other.m_handle = NULL;
			}
		}

		Void ServiceHandle::Finalizer(SC_HANDLE handle)
		{
			::CloseServiceHandle(handle);
		}
	}
}