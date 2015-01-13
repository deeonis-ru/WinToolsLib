#pragma once
#include "..\Types.h"

#include <memory>

namespace WinToolsLib { namespace Signals
{
	class Track;
	typedef std::shared_ptr<Bool> SignalExistFlag;
	typedef UInt64 SlotId;

	class SignalBase
	{
	public:
		SignalBase();
		virtual ~SignalBase();

		virtual Void Disconnect(SlotId id) {};
		virtual Void DisconnectAll() {};

		const SignalExistFlag& GetExistFlag() const;

	protected:
		SlotId m_lastId;
		SignalExistFlag m_existFlag;
	};

	inline SignalBase::SignalBase() :
		m_lastId(0)
	{
	}

	inline SignalBase::~SignalBase()
	{
		DisconnectAll();
	}

	inline const SignalExistFlag& SignalBase::GetExistFlag() const
	{
		return m_existFlag;
	}

} }