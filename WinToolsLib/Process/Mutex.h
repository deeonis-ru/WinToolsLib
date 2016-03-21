#pragma once
#include "..\Types.h"
#include "..\Handles\Handle.h"
#include "IWaitable.h"

namespace WinToolsLib { namespace Process
{
	using namespace Handles;

	class Mutex : public IWaitable
	{
	public:
		Mutex();
		Mutex(Mutex&& other);
		~Mutex();

		Void Create(Bool initialState, const TChar* name = nullptr);
		Void Open(Bool modifyState, Bool inheritHandle, const TChar* name);

		inline Bool IsExistBefore() const;
		Bool IsValid() const;
		Void Set();
		Void Release();

		Void Wait() const override;
		Void Wait(UInt32 milliseconds) const override;

		operator HANDLE() const;

	private:
		Handle m_handle;
		Bool m_isExist;
	};

	inline Mutex::operator HANDLE() const
	{
		return m_handle;
	}

	Bool Mutex::IsExistBefore() const
	{
		return m_isExist;
	}
}}


