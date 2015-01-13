#pragma once
#include "../Types.h"

#include <list>
#include <assert.h>

#include "ConnectionBase.h"

namespace WinToolsLib { namespace Signals
{
	template <class ClassType>
	class ClassConnection0 : public ConnectionBase0
	{
	public:
		typedef Void (ClassType::*ClassFunction)();
		ClassConnection0(SlotId id, ClassType* object, ClassFunction memberFunction);

		Void AddCallableObject(ClassFunction function);

		Void Emit() override;

	private:
		ClassType* m_classObject;
		std::list<ClassFunction> m_functionList;
	};

	template <class ClassType>
	ClassConnection0<ClassType>::ClassConnection0(SlotId id, ClassType* object, ClassFunction memberFunction) :
		m_classObject(object),
		ConnectionBase0(id)
	{
		assert(memberFunction);
		m_functionList.push_back(memberFunction);
	}

	template <class ClassType>
	Void ClassConnection0<ClassType>::AddCallableObject(ClassFunction function)
	{
		assert(function);
		m_functionList.push_back(function);
	}

	template <class ClassType>
	Void ClassConnection0<ClassType>::Emit()
	{
		assert(m_classObject);

		for (auto function : m_functionList)
			(m_classObject->*function)();
	}

	template <class ClassType, typename Arg1Type>
	class ClassConnection1 : public ConnectionBase1<Arg1Type>
	{
	public:
		typedef Void (ClassType::*ClassFunction)(Arg1Type);
		ClassConnection1(SlotId id, ClassType* object, ClassFunction memberFunction);

		Void AddCallableObject(ClassFunction function);

		Void Emit(Arg1Type arg1) override;

	private:
		ClassType* m_classObject;
		std::list<ClassFunction> m_functionList;
	};

	template <class ClassType, typename Arg1Type>
	ClassConnection1<ClassType, Arg1Type>::ClassConnection1(SlotId id, ClassType* object, ClassFunction memberFunction) :
		m_classObject(object),
		ConnectionBase1(id)
	{
		assert(memberFunction);
		m_functionList.push_back(memberFunction);
	}

	template <class ClassType, typename Arg1Type>
	Void ClassConnection1<ClassType, Arg1Type>::AddCallableObject(ClassFunction function)
	{
		assert(function);
		m_functionList.push_back(function);
	}

	template <class ClassType, typename Arg1Type>
	Void ClassConnection1<ClassType, Arg1Type>::Emit(Arg1Type arg1)
	{
		assert(m_classObject);

		for (auto function : m_functionList)
			(m_classObject->*function)(arg1);
	}

	template <class ClassType, typename Arg1Type, typename Arg2Type>
	class ClassConnection2 : public ConnectionBase2<Arg1Type, Arg2Type>
	{
	public:
		typedef Void (ClassType::*ClassFunction)(Arg1Type, Arg2Type);
		ClassConnection2(SlotId id, ClassType* object, Void(ClassType::* memberFunction)(Arg1Type, Arg2Type));

		Void AddCallableObject(ClassFunction function);

		Void Emit(Arg1Type arg1, Arg2Type arg2) override;

	private:
		ClassType* m_classObject;
		std::list<ClassFunction> m_functionList;
	};

	template <class ClassType, typename Arg1Type, typename Arg2Type>
	ClassConnection2<ClassType, Arg1Type, Arg2Type>::ClassConnection2(
		SlotId id, 
		ClassType* object,
		ClassFunction memberFunction) :
		m_classObject(object),
		ConnectionBase2(id)
	{
		assert(memberFunction);
		m_functionList.push_back(memberFunction);
	}

	template <class ClassType, typename Arg1Type, typename Arg2Type>
	Void ClassConnection2<ClassType, Arg1Type, Arg2Type>::AddCallableObject(ClassFunction function)
	{
		assert(function);
		m_functionList.push_back(function);
	}

	template <class ClassType, typename Arg1Type, typename Arg2Type>
	Void ClassConnection2<ClassType, Arg1Type, Arg2Type>::Emit(Arg1Type arg1, Arg2Type arg2)
	{
		assert(m_classObject);

		for (auto function : m_functionList)
			(m_classObject->*function)(arg1, arg2);
	}

	template <class ClassType, typename Arg1Type, typename Arg2Type, typename Arg3Type>
	class ClassConnection3 : public ConnectionBase3<Arg1Type, Arg2Type, Arg3Type>
	{
	public:
		typedef Void (ClassType::*ClassFunction)(Arg1Type, Arg2Type, Arg3Type);
		ClassConnection3(SlotId id, ClassType* object, ClassFunction memberFunction);

		Void AddCallableObject(ClassFunction function);

		Void Emit(Arg1Type arg1, Arg2Type arg2, Arg3Type arg3) override;

	private:
		ClassType* m_classObject;
		std::list<ClassFunction> m_functionList;
	};

	template <class ClassType, typename Arg1Type, typename Arg2Type, typename Arg3Type>
	ClassConnection3<ClassType, Arg1Type, Arg2Type, Arg3Type>::
	ClassConnection3(SlotId id, ClassType* object, ClassFunction memberFunction) :
		m_classObject(object),
		ConnectionBase3(id)
	{
		assert(memberFunction);
		m_functionList.push_back(memberFunction);
	}

	template <class ClassType, typename Arg1Type, typename Arg2Type, typename Arg3Type>
	Void ClassConnection3<ClassType, Arg1Type, Arg2Type, Arg3Type>::AddCallableObject(ClassFunction function)
	{
		assert(function);
		m_functionList.push_back(function);
	}

	template <class ClassType, typename Arg1Type, typename Arg2Type, typename Arg3Type>
	Void ClassConnection3<ClassType, Arg1Type, Arg2Type, Arg3Type>::Emit(Arg1Type arg1, Arg2Type arg2, Arg3Type arg3)
	{
		assert(m_classObject);

		for (auto function : m_functionList)
			(m_classObject->*function)(arg1, arg2, arg3);
	}

	template <class ClassType, typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type>
	class ClassConnection4 : public ConnectionBase4<Arg1Type, Arg2Type, Arg3Type, Arg4Type>
	{
	public:
		typedef Void (ClassType::*ClassFunction)(Arg1Type, Arg2Type, Arg3Type, Arg4Type);
		ClassConnection4(SlotId id, ClassType* object, ClassFunction memberFunction);

		Void AddCallableObject(ClassFunction function);

		Void Emit(Arg1Type arg1, Arg2Type arg2, Arg3Type arg3, Arg4Type arg4) override;

	private:
		ClassType* m_classObject;
		std::list<ClassFunction> m_functionList;
	};

	template <class ClassType, typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type>
	ClassConnection4<ClassType, Arg1Type, Arg2Type, Arg3Type, Arg4Type>::
	ClassConnection4(SlotId id, ClassType* object, Void (ClassType::* memberFunction)(Arg1Type, Arg2Type, Arg3Type, Arg4Type)) :
		m_classObject(object),
		ConnectionBase4(id)
	{
		assert(memberFunction);
		m_functionList.push_back(memberFunction);
	}

	template <class ClassType, typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type>
	Void ClassConnection4<ClassType, Arg1Type, Arg2Type, Arg3Type, Arg4Type>::AddCallableObject(ClassFunction function)
	{
		assert(function);
		m_functionList.push_back(function);
	}

	template <class ClassType, typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type>
	Void ClassConnection4<ClassType, Arg1Type, Arg2Type, Arg3Type, Arg4Type>::
	Emit(Arg1Type arg1, Arg2Type arg2, Arg3Type arg3, Arg4Type arg4)
	{
		assert(m_classObject);

		for (auto function : m_functionList)
			(m_classObject->*function)(arg1, arg2, arg3, arg4);
	}
} }