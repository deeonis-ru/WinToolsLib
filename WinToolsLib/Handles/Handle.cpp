#include "Handle.h"
#include "..\Exception.h"

namespace WinToolsLib
{
	namespace Handles
	{
		Handle::Handle() :
			Base(Handle::Finalizer)
		{
		}

		Handle::Handle(HANDLE handle) :
			Base(Handle::Finalizer, handle)
		{
		}

		Handle::Handle(Handle&& other) :
			Base(Handle::Finalizer)
		{
			MoveFrom(std::move(other));
		}

		Handle::Handle(const Handle& other) :
			Base(Handle::Finalizer)
		{
			CopyFrom(other);
		}

		Bool Handle::IsValid() const
		{
			if (IsNull())
				return False;

			if (INVALID_HANDLE_VALUE == m_handle)
				return False;

			return True;
		}

		Void Handle::Duplicate(HANDLE handle)
		{
			Close();
			BOOL success = ::DuplicateHandle(
				::GetCurrentProcess(), 
				handle, 
				::GetCurrentProcess(),
				&m_handle, 
				0,
				FALSE,
				DUPLICATE_SAME_ACCESS);

			if (!success)
			{
				THROW_LAST_ERROR_EXCEPTION();
			}
		}

		Handle& Handle::operator=(Handle&& other)
		{
			MoveFrom(std::move(other));
			return *this;
		}

		Handle& Handle::operator=(const Handle& other)
		{
			CopyFrom(other);
			return *this;
		}

		Void Handle::MoveFrom(Handle&& other)
		{
			if (this != &other)
			{
				Reset(other.m_handle);
				other.m_handle = NULL;
			}
		}

		Void Handle::CopyFrom(const Handle& other)
		{
			if (this != &other)
			{
				if (other.IsValid())
				{
					Duplicate(other.m_handle);
				}
			}
		}

		Void Handle::Finalizer(HANDLE handle)
		{
			::CloseHandle(handle);
		}
	}
}