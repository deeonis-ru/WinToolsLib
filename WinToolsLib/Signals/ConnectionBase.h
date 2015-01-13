#pragma once
#include "../Types.h"
#include "Trackable.h"

#include <memory>

namespace WinToolsLib { namespace Signals
{
	class ConnectionBase
	{
	public:
		ConnectionBase(SlotId id);
		virtual ~ConnectionBase();

		SlotId GetSlotId() const;

	private:
		SlotId m_slotId;
	};

	inline SlotId ConnectionBase::GetSlotId() const
	{
		return m_slotId;
	}

	class ConnectionBase0 : public ConnectionBase
	{
	public:
		ConnectionBase0(SlotId id);
		virtual ~ConnectionBase0();

		virtual Void Emit() = 0;
	};

	template<typename Arg1Type>
	class ConnectionBase1 : public ConnectionBase
	{
	public:
		ConnectionBase1(SlotId id);
		virtual ~ConnectionBase1();

		virtual Void Emit(Arg1Type arg1) = 0;
	};

	template <typename Arg1Type>
	ConnectionBase1<Arg1Type>::ConnectionBase1(SlotId id) :
		ConnectionBase(id)
	{
	}

	template <typename Arg1Type>
	ConnectionBase1<Arg1Type>::~ConnectionBase1()
	{
	}

	template<typename Arg1Type, typename Arg2Type>
	class ConnectionBase2 : public ConnectionBase
	{
	public:
		ConnectionBase2(SlotId id);
		virtual ~ConnectionBase2();

		virtual Void Emit(Arg1Type arg1, Arg2Type arg2) = 0;
	};

	template <typename Arg1Type, typename Arg2Type>
	ConnectionBase2<Arg1Type, Arg2Type>::ConnectionBase2(SlotId id) :
		ConnectionBase(id)
	{
	}

	template <typename Arg1Type, typename Arg2Type>
	ConnectionBase2<Arg1Type, Arg2Type>::~ConnectionBase2()
	{
	}

	template<typename Arg1Type, typename Arg2Type, typename Arg3Type>
	class ConnectionBase3 : public ConnectionBase
	{
	public:
		ConnectionBase3(SlotId id);
		virtual ~ConnectionBase3();

		virtual Void Emit(Arg1Type arg1, Arg2Type arg2, Arg3Type arg3) = 0;
	};

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type>
	ConnectionBase3<Arg1Type, Arg2Type, Arg3Type>::ConnectionBase3(SlotId id) :
		ConnectionBase(id)
	{
	}

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type>
	ConnectionBase3<Arg1Type, Arg2Type, Arg3Type>::~ConnectionBase3()
	{
	}

	template<typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type>
	class ConnectionBase4 : public ConnectionBase
	{
	public:
		ConnectionBase4(SlotId id);
		virtual ~ConnectionBase4();

		virtual Void Emit(Arg1Type arg1, Arg2Type arg2, Arg3Type arg3, Arg4Type arg4) = 0;
	};

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type>
	ConnectionBase4<Arg1Type, Arg2Type, Arg3Type, Arg4Type>::ConnectionBase4(SlotId id) :
		ConnectionBase(id)
	{
	}

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type>
	ConnectionBase4<Arg1Type, Arg2Type, Arg3Type, Arg4Type>::~ConnectionBase4()
	{
	}
} }