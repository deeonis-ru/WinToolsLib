#include "FindHandle.h"

namespace WinToolsLib
{
	namespace Handles
	{
		FindHandle::FindHandle() :
			Base(FindHandle::Finalizer)
		{
		}

		FindHandle::FindHandle(HANDLE handle) :
			Base(FindHandle::Finalizer, handle)
		{
		}

		FindHandle::FindHandle(FindHandle&& other) :
			Base(FindHandle::Finalizer)
		{
			MoveFrom(std::move(other));
		}

		Bool FindHandle::IsValid() const
		{
			if (IsNull())
				return False;

			if (INVALID_HANDLE_VALUE == m_handle)
				return False;

			return True;
		}

		FindHandle& FindHandle::operator=(FindHandle&& other)
		{
			MoveFrom(std::move(other));
			return *this;
		}

		Void FindHandle::MoveFrom(FindHandle&& other)
		{
			if (this != &other)
			{
				Reset(other.m_handle);
				other.m_handle = NULL;
			}
		}

		Void FindHandle::Finalizer(HANDLE handle)
		{
			::FindClose(handle);
		}
	}
}