#pragma once
#include "Types.h"
#include "IWaitable.h"
#include "Handles\Handle.h"

namespace WinToolsLib
{
	using namespace Handles;

	class Event : public IWaitable
	{
	public:
		Event();
		Event(Event&& other);
		~Event();

		Void Create(Bool manualReset, Bool initialState, const TChar* name = nullptr);
		Void Open(Bool modifyState, Bool inheritHandle, const TChar* name);

		Bool IsValid() const;
		Void Set();
		Void Reset();

		Void Wait() const override;
		Void Wait(UInt32 milliseconds) const override;

		operator HANDLE() const;

	private:
		Handle m_handle;
	};

	inline Event::operator HANDLE() const
	{
		return m_handle;
	}
}
