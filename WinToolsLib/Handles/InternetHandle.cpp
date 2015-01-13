#include "InternetHandle.h"

namespace WinToolsLib
{
	namespace Handles
	{
		InternetHandle::InternetHandle() :
			Base(InternetHandle::Finalizer)
		{
		}

		InternetHandle::InternetHandle(HINTERNET handle) :
			Base(InternetHandle::Finalizer, handle)
		{
		}

		InternetHandle::InternetHandle(InternetHandle&& other) :
			Base(InternetHandle::Finalizer)
		{
			MoveFrom(std::move(other));
		}

		InternetHandle& InternetHandle::operator=(InternetHandle&& other)
		{
			MoveFrom(std::move(other));
			return *this;
		}

		Void InternetHandle::MoveFrom(InternetHandle&& other)
		{
			if (this != &other)
			{
				Reset(other.m_handle);
				other.m_handle = NULL;
			}
		}

		Void InternetHandle::Finalizer(HINTERNET handle)
		{
			::InternetCloseHandle(handle);
		}
	}
}