#include "FsRedirection.h"
#include "..\DynamicFunction.h"
#include <Windows.h>

namespace WinToolsLib
{
	namespace Wow64
	{
		FsRedirection::FsRedirection() :
			m_shouldRevert(False),
			m_oldValue(nullptr)
		{
		}

		FsRedirection::~FsRedirection()
		{
			Revert();
		}

		FsRedirection::FsRedirection(FsRedirection&& other) :
			m_shouldRevert(other.m_shouldRevert),
			m_oldValue(other.m_oldValue)
		{
			other.m_shouldRevert = False;
			other.m_oldValue = nullptr;
		}

		FsRedirection& FsRedirection::operator=(FsRedirection&& other)
		{
			Revert();

			m_shouldRevert = other.m_shouldRevert;
			m_oldValue = other.m_oldValue;

			other.m_shouldRevert = False;
			other.m_oldValue = nullptr;

			return *this;
		}

		Void FsRedirection::Disable()
		{
			typedef BOOL (WINAPI *Fn)(PVOID* OldValue);
		
			static const auto fn = DynamicFunction<Fn>("kernel32", "Wow64DisableWow64FsRedirection");
			if (fn)
			{
				fn(&m_oldValue);
				m_shouldRevert = True;
			}
		}

		Void FsRedirection::Revert()
		{
			typedef BOOL (WINAPI *Fn)(PVOID OldValue);

			if (!m_shouldRevert)
			{
				return;
			}

			static const auto fn = DynamicFunction<Fn>("kernel32", "Wow64RevertWow64FsRedirection");
			if (fn)
			{
				fn(m_oldValue);
				m_shouldRevert = False;
			}
		}
	}
}
