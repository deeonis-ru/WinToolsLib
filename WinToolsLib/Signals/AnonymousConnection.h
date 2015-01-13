#pragma once
#include "../Types.h"

#include <functional>

#include "ConnectionBase.h"
#include "Trackable.h"

namespace WinToolsLib { namespace Signals
{
	class AnonymousConnection0 : public ConnectionBase0
	{
		typedef std::function<Void()> AnonymousFunction;
	public:
		AnonymousConnection0(SlotId id, AnonymousFunction& function);

		Void Emit() override;

	private:
		AnonymousFunction m_function;
	};

	inline Void AnonymousConnection0::Emit()
	{
		m_function();
	}

	template <typename Arg1Type>
	class AnonymousConnection1 : public ConnectionBase1<Arg1Type>
	{
		typedef std::function<Void(Arg1Type)> AnonymousFunction;
	public:
		AnonymousConnection1(SlotId id, AnonymousFunction& function);

		Void Emit(Arg1Type arg1) override;

	private:
		AnonymousFunction m_function;
	};

	template <typename Arg1Type>
	AnonymousConnection1<Arg1Type>::AnonymousConnection1(SlotId id, AnonymousFunction& function) :
		m_function(function),
		ConnectionBase1(id)
	{
	}

	template <typename Arg1Type>
	Void AnonymousConnection1<Arg1Type>::Emit(Arg1Type arg1)
	{
		m_function(arg1);
	}

	template <typename Arg1Type, typename Arg2Type>
	class AnonymousConnection2 : public ConnectionBase2<Arg1Type, Arg2Type>
	{
		typedef std::function<Void(Arg1Type, Arg2Type)> AnonymousFunction;
	public:
		AnonymousConnection2(SlotId id, AnonymousFunction& function);

		Void Emit(Arg1Type arg1, Arg2Type arg2) override;

	private:
		AnonymousFunction m_function;
	};

	template <typename Arg1Type, typename Arg2Type>
	AnonymousConnection2<Arg1Type, Arg2Type>::AnonymousConnection2(SlotId id, AnonymousFunction& function) :
		m_function(function),
		ConnectionBase2(id)
	{
	}

	template <typename Arg1Type, typename Arg2Type>
	Void AnonymousConnection2<Arg1Type, Arg2Type>::Emit(Arg1Type arg1, Arg2Type arg2)
	{
		m_function(arg1, arg2);
	}

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type>
	class AnonymousConnection3 : public ConnectionBase3<Arg1Type, Arg2Type, Arg3Type>
	{
		typedef std::function<Void(Arg1Type, Arg2Type, Arg3Type)> AnonymousFunction;
	public:
		AnonymousConnection3(SlotId id, AnonymousFunction& function);

		Void Emit(Arg1Type arg1, Arg2Type arg2, Arg3Type arg3) override;

	private:
		AnonymousFunction m_function;
	};

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type>
	AnonymousConnection3<Arg1Type, Arg2Type, Arg3Type>::
	AnonymousConnection3(SlotId id, AnonymousFunction& function) :
		m_function(function),
		ConnectionBase3(id)
	{
	}

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type>
	Void AnonymousConnection3<Arg1Type, Arg2Type, Arg3Type>::Emit(Arg1Type arg1, Arg2Type arg2, Arg3Type arg3)
	{
		m_function(arg1, arg2, arg3);
	}

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type>
	class AnonymousConnection4 : public ConnectionBase4<Arg1Type, Arg2Type, Arg3Type, Arg4Type>
	{
		typedef std::function<Void(Arg1Type, Arg2Type, Arg3Type, Arg4Type)> AnonymousFunction;
	public:
		AnonymousConnection4(SlotId id, AnonymousFunction& function);

		Void Emit(Arg1Type arg1, Arg2Type arg2, Arg3Type arg3, Arg4Type arg4) override;

	private:
		AnonymousFunction m_function;
	};

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type>
	AnonymousConnection4<Arg1Type, Arg2Type, Arg3Type, Arg4Type>::
	AnonymousConnection4(SlotId id, AnonymousFunction& function) :
		m_function(function),
		ConnectionBase4(id)
	{
	}

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type>
	Void AnonymousConnection4<Arg1Type, Arg2Type, Arg3Type, Arg4Type>::Emit(Arg1Type arg1, Arg2Type arg2, Arg3Type arg3, Arg4Type arg4)
	{
		m_function(arg1, arg2, arg3, arg4);
	}
} }