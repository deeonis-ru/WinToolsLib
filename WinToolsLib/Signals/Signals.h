#pragma once
#include "..\Types.h"

#include "SignalBase.h"
#include "Trackable.h"
#include "ClassConnection.h"
#include "AnonymousConnection.h"
#include "MultiThreadModes.h"

#include <algorithm>
#include <list>
#include <mutex>

namespace WinToolsLib { namespace Signals
{
	template <class ThreadMode = OSHI_SIGNALS_DEFAULT_THREAD_MODE>
	class Signal0 : public SignalBase
	{
		typedef ConnectionBase0 SlotConnection;
		typedef AnonymousConnection0 AnonymousConnection;
		typedef AnonymousTrack<ThreadMode> TrackAnonymous;

	public:	
		typedef std::unique_ptr<TrackAnonymous> TrackAnonymousPtr;
		typedef std::shared_ptr<ThreadMode> SignalExistMutex;
		typedef std::function<Void()> AnonymousHandler;

		Signal0();
		~Signal0();

		template <class ClassType>
		Void Connect(ClassType* object, Void (ClassType::*memberFunction)());
		TrackAnonymousPtr Connect(AnonymousHandler function);

		Void Disconnect(SlotId id) override;
		Void DisconnectAll() override;

		Void Emit();
		Void operator()();

	private:
		std::list<std::unique_ptr<SlotConnection>> m_slotList;
		ThreadMode m_slotListMutex;
		SignalExistMutex m_existMutex;
	};

	template <class ThreadMode>
	Signal0<ThreadMode>::Signal0()
	{
		m_existMutex = std::make_shared<ThreadMode>();
		m_existFlag = std::make_shared<Bool>(True);
	}

	template <class ThreadMode>
	Signal0<ThreadMode>::~Signal0()
	{
		std::lock_guard<ThreadMode> lock(*m_existMutex);
		*m_existFlag = False;
	}

	template <class ThreadMode>
	template <class ClassType>
	Void Signal0<ThreadMode>::Connect(ClassType* object, Void (ClassType::*memberFunction)())
	{
		std::lock_guard<ThreadMode> lock(m_slotListMutex);

		Bool needCreate = True;
		for (auto& conn : m_slotList)
		{
			const auto slotId = conn->GetSlotId();
			if (slotId == object->GetSlotId(this))
			{
				auto connPtr = reinterpret_cast<ClassConnection0<ClassType>*>(conn.get());
				if (connPtr)
				{
					needCreate = False;
					connPtr->AddCallableObject(memberFunction);
					break;
				}
			}
		}

		if (needCreate)
		{
			auto conn = std::unique_ptr<SlotConnection>(new ClassConnection0<ClassType>(m_lastId, object, memberFunction));
			object->SignalConnected(this, m_lastId, m_existMutex);
			m_slotList.push_back(std::move(conn));
			m_lastId++;
		}
	}

	template <class ThreadMode>
	typename Signal0<ThreadMode>::TrackAnonymousPtr Signal0<ThreadMode>::Connect(AnonymousHandler function)
	{
		std::lock_guard<ThreadMode> lock(m_slotListMutex);

		auto track = std::unique_ptr<TrackAnonymous>(new TrackAnonymous(this, m_lastId, m_existMutex));
		auto conn = std::unique_ptr<SlotConnection>(new AnonymousConnection(m_lastId, function));
		m_slotList.push_back(std::move(conn));
		m_lastId++;

		return std::move(track);
	}

	template <class ThreadMode>
	Void Signal0<ThreadMode>::Disconnect(SlotId id)
	{
		std::lock_guard<ThreadMode> lock(m_slotListMutex);

		auto newEnd = std::remove_if(std::begin(m_slotList), std::end(m_slotList), [id](const std::unique_ptr<SlotConnection>& conn)
		{
			return conn->GetSlotId() == id;
		});
		m_slotList.erase(newEnd, std::end(m_slotList));
	}

	template <class ThreadMode>
	Void Signal0<ThreadMode>::DisconnectAll()
	{
		std::lock_guard<ThreadMode> lock(m_slotListMutex);
		m_slotList.clear();
	}

	template <class ThreadMode>
	Void Signal0<ThreadMode>::Emit()
	{
		std::lock_guard<ThreadMode> lock(m_slotListMutex);

		for (auto& conn : m_slotList)
		{
			conn->Emit();
		}
	}

	template <class ThreadMode>
	Void Signal0<ThreadMode>::operator()()
	{
		Emit();
	}

	template <typename Arg1Type, class ThreadMode = OSHI_SIGNALS_DEFAULT_THREAD_MODE>
	class Signal1 : public SignalBase
	{
		typedef ConnectionBase1<Arg1Type> SlotConnection;
		typedef AnonymousConnection1<Arg1Type> AnonymousConnection;
		typedef AnonymousTrack<ThreadMode> TrackAnonymous;

	public:	
		typedef std::unique_ptr<TrackAnonymous> TrackAnonymousPtr;
		typedef std::shared_ptr<ThreadMode> SignalExistMutex;
		typedef std::function<Void(Arg1Type)> AnonymousHandler;

		Signal1();
		~Signal1();

		template <class ClassType>
		Void Connect(ClassType* object, Void (ClassType::*memberFunction)(Arg1Type));
		TrackAnonymousPtr Connect(AnonymousHandler function);

		Void Disconnect(SlotId id) override;
		Void DisconnectAll() override;

		Void Emit(Arg1Type arg1);
		Void operator()(Arg1Type arg1);

	private:
		std::list<std::unique_ptr<SlotConnection>> m_slotList;
		ThreadMode m_slotListMutex;
		SignalExistMutex m_existMutex;
	};

	template <typename Arg1Type, class ThreadMode>
	Signal1<Arg1Type, ThreadMode>::Signal1()
	{
		m_existMutex = std::make_shared<ThreadMode>();
		m_existFlag = std::make_shared<Bool>(True);
	}

	template <typename Arg1Type, class ThreadMode>
	Signal1<Arg1Type, ThreadMode>::~Signal1()
	{
		std::lock_guard<ThreadMode> lock(*m_existMutex);
		*m_existFlag = False;
	}

	template <typename Arg1Type, class ThreadMode>
	template <class ClassType>
	Void Signal1<Arg1Type, ThreadMode>::Connect(ClassType* object, Void (ClassType::*memberFunction)(Arg1Type))
	{
		std::lock_guard<ThreadMode> lock(m_slotListMutex);

		Bool needCreate = True;
		for (auto& conn : m_slotList)
		{
			const auto slotId = conn->GetSlotId();
			if (slotId == object->GetSlotId(this))
			{
				auto connPtr = reinterpret_cast<ClassConnection1<ClassType, Arg1Type>*>(conn.get());
				if (connPtr)
				{
					needCreate = False;
					connPtr->AddCallableObject(memberFunction);
					break;
				}
			}
		}

		if (needCreate)
		{
			auto conn = std::unique_ptr<SlotConnection>(new ClassConnection1<ClassType, Arg1Type>(m_lastId, object, memberFunction));
			object->SignalConnected(this, m_lastId, m_existMutex);
			m_slotList.push_back(std::move(conn));
			m_lastId++;
		}
	}

	template <typename Arg1Type, class ThreadMode>
	typename Signal1<Arg1Type, ThreadMode>::TrackAnonymousPtr
	Signal1<Arg1Type, ThreadMode>::Connect(AnonymousHandler function)
	{
		std::lock_guard<ThreadMode> lock(m_slotListMutex);

		auto track = std::unique_ptr<TrackAnonymous>(new TrackAnonymous(this, m_lastId, m_existMutex));
		auto conn = std::unique_ptr<SlotConnection>(new AnonymousConnection(m_lastId, function));
		m_slotList.push_back(std::move(conn));
		
		return std::move(track);
	}

	template <typename Arg1Type, class ThreadMode>
	Void Signal1<Arg1Type, ThreadMode>::Disconnect(SlotId id)
	{
		std::lock_guard<ThreadMode> lock(m_slotListMutex);

		auto newEnd = std::remove_if(std::begin(m_slotList), std::end(m_slotList), [id](const std::unique_ptr<SlotConnection>& conn)
		{
			return conn->GetSlotId() == id;
		});
		m_slotList.erase(newEnd, std::end(m_slotList));
	}

	template <typename Arg1Type, class ThreadMode>
	Void Signal1<Arg1Type, ThreadMode>::DisconnectAll()
	{
		std::lock_guard<ThreadMode> lock(m_slotListMutex);
		m_slotList.clear();
	}

	template <typename Arg1Type, class ThreadMode>
	Void Signal1<Arg1Type, ThreadMode>::Emit(Arg1Type arg1)
	{
		std::lock_guard<ThreadMode> lock(m_slotListMutex);

		for (auto& conn : m_slotList)
		{
			conn->Emit(arg1);
		}
	}

	template <typename Arg1Type, class ThreadMode>
	Void Signal1<Arg1Type, ThreadMode>::operator()(Arg1Type arg1)
	{
		Emit(arg1);
	}

	template <typename Arg1Type, typename Arg2Type, class ThreadMode = OSHI_SIGNALS_DEFAULT_THREAD_MODE>
	class Signal2 : public SignalBase
	{
		typedef ConnectionBase2<Arg1Type, Arg2Type> SlotConnection;
		typedef AnonymousConnection2<Arg1Type, Arg2Type> AnonymousConnection;
		typedef AnonymousTrack<ThreadMode> TrackAnonymous;

	public:	
		typedef std::unique_ptr<TrackAnonymous> TrackAnonymousPtr;
		typedef std::shared_ptr<ThreadMode> SignalExistMutex;
		typedef std::function<Void(Arg1Type, Arg2Type)> AnonymousHandler;

		Signal2();
		~Signal2();

		template <class ClassType>
		Void Connect(ClassType* object, Void (ClassType::*memberFunction)(Arg1Type, Arg2Type));
		TrackAnonymousPtr Connect(AnonymousHandler function);

		Void Disconnect(SlotId id) override;
		Void DisconnectAll() override;

		Void Emit(Arg1Type arg1, Arg2Type arg2);
		Void operator()(Arg1Type arg1, Arg2Type arg2);

	private:
		std::list<std::unique_ptr<SlotConnection>> m_slotList;
		ThreadMode m_slotListMutex;
		SignalExistMutex m_existMutex;
	};

	template <typename Arg1Type, typename Arg2Type, class ThreadMode>
	Signal2<Arg1Type, Arg2Type, ThreadMode>::Signal2()
	{
		m_existMutex = std::make_shared<ThreadMode>();
		m_existFlag = std::make_shared<Bool>(True);
	}

	template <typename Arg1Type, typename Arg2Type, class ThreadMode>
	Signal2<Arg1Type, Arg2Type, ThreadMode>::~Signal2()
	{
		std::lock_guard<ThreadMode> lock(*m_existMutex);
		*m_existFlag = False;
	}

	template <typename Arg1Type, typename Arg2Type, class ThreadMode>
	template <class ClassType>
	Void Signal2<Arg1Type, Arg2Type, ThreadMode>::Connect(ClassType* object, Void (ClassType::*memberFunction)(Arg1Type, Arg2Type))
	{
		std::lock_guard<ThreadMode> lock(m_slotListMutex);

		Bool needCreate = True;
		for (auto& conn : m_slotList)
		{
			const auto slotId = conn->GetSlotId();
			if (slotId == object->GetSlotId(this))
			{
				auto connPtr = reinterpret_cast<ClassConnection2<ClassType, Arg1Type, Arg2Type>*>(conn.get());
				if (connPtr)
				{
					needCreate = False;
					connPtr->AddCallableObject(memberFunction);
					break;
				}
			}
		}

		if (needCreate)
		{
			auto conn = std::unique_ptr<SlotConnection>(new ClassConnection2<ClassType, Arg1Type, Arg2Type>(m_lastId, object, memberFunction));
			object->SignalConnected(this, m_lastId, m_existMutex);
			m_slotList.push_back(std::move(conn));
			m_lastId++;
		}
	}

	template <typename Arg1Type, typename Arg2Type, class ThreadMode>
	typename Signal2<Arg1Type, Arg2Type, ThreadMode>::TrackAnonymousPtr 
	Signal2<Arg1Type, Arg2Type, ThreadMode>::Connect(AnonymousHandler function)
	{
		std::lock_guard<ThreadMode> lock(m_slotListMutex);

		auto track = std::unique_ptr<TrackAnonymous>(new TrackAnonymous(this, m_lastId, m_existMutex));
		auto conn = std::unique_ptr<SlotConnection>(new AnonymousConnection(m_lastId, function));
		m_slotList.push_back(std::move(conn));
		m_lastId++;
		
		return std::move(track);
	}

	template <typename Arg1Type, typename Arg2Type, class ThreadMode>
	Void Signal2<Arg1Type, Arg2Type, ThreadMode>::Disconnect(SlotId id)
	{
		std::lock_guard<ThreadMode> lock(m_slotListMutex);

		auto newEnd = std::remove_if(std::begin(m_slotList), std::end(m_slotList), [id](const std::unique_ptr<SlotConnection>& conn)
		{
			return conn->GetSlotId() == id;
		});
		m_slotList.erase(newEnd, std::end(m_slotList));
	}

	template <typename Arg1Type, typename Arg2Type, class ThreadMode>
	Void Signal2<Arg1Type, Arg2Type, ThreadMode>::DisconnectAll()
	{
		std::lock_guard<ThreadMode> lock(m_slotListMutex);
		m_slotList.clear();
	}

	template <typename Arg1Type, typename Arg2Type, class ThreadMode>
	Void Signal2<Arg1Type, Arg2Type, ThreadMode>::Emit(Arg1Type arg1, Arg2Type arg2)
	{
		std::lock_guard<ThreadMode> lock(m_slotListMutex);

		for (auto& conn : m_slotList)
		{
			conn->Emit(arg1, arg2);
		}
	}

	template <typename Arg1Type, typename Arg2Type, class ThreadMode>
	Void Signal2<Arg1Type, Arg2Type, ThreadMode>::operator()(Arg1Type arg1, Arg2Type arg2)
	{
		Emit(arg1, arg2);
	}

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type, class ThreadMode = OSHI_SIGNALS_DEFAULT_THREAD_MODE>
	class Signal3 : public SignalBase
	{
		typedef ConnectionBase3<Arg1Type, Arg2Type, Arg3Type> SlotConnection;
		typedef AnonymousConnection3<Arg1Type, Arg2Type, Arg3Type> AnonymousConnection;
		typedef AnonymousTrack<ThreadMode> TrackAnonymous;

	public:	
		typedef std::unique_ptr<TrackAnonymous> TrackAnonymousPtr;
		typedef std::shared_ptr<ThreadMode> SignalExistMutex;
		typedef std::function<Void(Arg1Type, Arg2Type, Arg3Type)> AnonymousHandler;

		Signal3();
		~Signal3();

		template <class ClassType>
		Void Connect(ClassType* object, Void (ClassType::*memberFunction)(Arg1Type, Arg2Type, Arg3Type));
		TrackAnonymousPtr Connect(AnonymousHandler function);

		Void Disconnect(SlotId id) override;
		Void DisconnectAll() override;

		Void Emit(Arg1Type arg1, Arg2Type arg2, Arg3Type arg3);
		Void operator()(Arg1Type arg1, Arg2Type arg2, Arg3Type arg3);
		
	private:
		std::list<std::unique_ptr<SlotConnection>> m_slotList;
		ThreadMode m_slotListMutex;
		SignalExistMutex m_existMutex;
	};

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type, class ThreadMode>
	Signal3<Arg1Type, Arg2Type, Arg3Type, ThreadMode>::Signal3()
	{
		m_existMutex = std::make_shared<ThreadMode>();
		m_existFlag = std::make_shared<Bool>(True);
	}

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type, class ThreadMode>
	Signal3<Arg1Type, Arg2Type, Arg3Type, ThreadMode>::~Signal3()
	{
		std::lock_guard<ThreadMode> lock(*m_existMutex);
		*m_existFlag = False;
	}

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type, class ThreadMode>
	template <class ClassType>
	Void Signal3<Arg1Type, Arg2Type, Arg3Type, ThreadMode>::Connect(
		ClassType* object, 
		Void (ClassType::*memberFunction)(Arg1Type, Arg2Type, Arg3Type))
	{
		std::lock_guard<ThreadMode> lock(m_slotListMutex);

		Bool needCreate = True;
		for (auto& conn : m_slotList)
		{
			const auto slotId = conn->GetSlotId();
			if (slotId == object->GetSlotId(this))
			{
				auto connPtr = reinterpret_cast<ClassConnection3<ClassType, Arg1Type, Arg2Type, Arg3Type>*>(conn.get());
				if (connPtr)
				{
					needCreate = False;
					connPtr->AddCallableObject(memberFunction);
					break;
				}
			}
		}

		if (needCreate)
		{
			auto conn = std::unique_ptr<SlotConnection>(
				new ClassConnection3<ClassType, Arg1Type, Arg2Type, Arg3Type>(m_lastId, object, memberFunction));
			object->SignalConnected(this, m_lastId, m_existMutex);
			m_slotList.push_back(std::move(conn));
			m_lastId++;
		}
	}

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type, class ThreadMode>
	typename Signal3<Arg1Type, Arg2Type, Arg3Type, ThreadMode>::TrackAnonymousPtr 
	Signal3<Arg1Type, Arg2Type, Arg3Type, ThreadMode>::Connect(AnonymousHandler function)
	{
		std::lock_guard<ThreadMode> lock(m_slotListMutex);

		auto track = std::unique_ptr<TrackAnonymous>(new TrackAnonymous(this, m_lastId, m_existMutex));
		auto conn = std::unique_ptr<SlotConnection>(new AnonymousConnection(m_lastId, function));
		m_slotList.push_back(std::move(conn));
		m_lastId++;
		
		return std::move(track);
	}

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type, class ThreadMode>
	Void Signal3<Arg1Type, Arg2Type, Arg3Type, ThreadMode>::Disconnect(SlotId id)
	{
		std::lock_guard<ThreadMode> lock(m_slotListMutex);

		auto newEnd = std::remove_if(std::begin(m_slotList), std::end(m_slotList), [id](const std::unique_ptr<SlotConnection>& conn)
		{
			return conn->GetSlotId() == id;
		});
		m_slotList.erase(newEnd, std::end(m_slotList));
	}

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type, class ThreadMode>
	Void Signal3<Arg1Type, Arg2Type, Arg3Type, ThreadMode>::DisconnectAll()
	{
		std::lock_guard<ThreadMode> lock(m_slotListMutex);
		m_slotList.clear();
	}

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type, class ThreadMode>
	Void Signal3<Arg1Type, Arg2Type, Arg3Type, ThreadMode>::Emit(Arg1Type arg1, Arg2Type arg2, Arg3Type arg3)
	{
		std::lock_guard<ThreadMode> lock(m_slotListMutex);

		for (auto& conn : m_slotList)
		{
			conn->Emit(arg1, arg2, arg3);
		}
	}

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type, class ThreadMode>
	Void Signal3<Arg1Type, Arg2Type, Arg3Type, ThreadMode>::operator()(Arg1Type arg1, Arg2Type arg2, Arg3Type arg3)
	{
		Emit(arg1, arg2, arg3);
	}

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type, class ThreadMode = OSHI_SIGNALS_DEFAULT_THREAD_MODE>
	class Signal4 : public SignalBase
	{
		typedef ConnectionBase4<Arg1Type, Arg2Type, Arg3Type, Arg4Type> SlotConnection;
		typedef AnonymousConnection4<Arg1Type, Arg2Type, Arg3Type, Arg4Type> AnonymousConnection;
		typedef AnonymousTrack<ThreadMode> TrackAnonymous;

	public:	
		typedef std::unique_ptr<TrackAnonymous> TrackAnonymousPtr;
		typedef std::shared_ptr<ThreadMode> SignalExistMutex;
		typedef std::function<Void(Arg1Type, Arg2Type, Arg3Type, Arg4Type)> AnonymousHandler;

		Signal4();
		~Signal4();

		template <class ClassType>
		Void Connect(ClassType* object, Void (ClassType::*memberFunction)(Arg1Type, Arg2Type, Arg3Type, Arg4Type));
		TrackAnonymousPtr Connect(AnonymousHandler function);

		Void Disconnect(SlotId id) override;
		Void DisconnectAll() override;

		Void Emit(Arg1Type arg1, Arg2Type arg2, Arg3Type arg3, Arg4Type arg4);
		Void operator()(Arg1Type arg1, Arg2Type arg2, Arg3Type arg3, Arg4Type arg4);
		
	private:
		std::list<std::unique_ptr<SlotConnection>> m_slotList;
		ThreadMode m_slotListMutex;
		SignalExistMutex m_existMutex;
	};

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type, class ThreadMode>
	Signal4<Arg1Type, Arg2Type, Arg3Type, Arg4Type, ThreadMode>::Signal4()
	{
		m_existMutex = std::make_shared<ThreadMode>();
		m_existFlag = std::make_shared<Bool>(True);
	}

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type, class ThreadMode>
	Signal4<Arg1Type, Arg2Type, Arg3Type, Arg4Type, ThreadMode>::~Signal4()
	{
		std::lock_guard<ThreadMode> lock(*m_existMutex);
		*m_existFlag = False;
	}

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type, class ThreadMode>
	template <class ClassType>
	Void Signal4<Arg1Type, Arg2Type, Arg3Type, Arg4Type, ThreadMode>::Connect(
		ClassType* object, 
		Void (ClassType::*memberFunction)(Arg1Type, Arg2Type, Arg3Type, Arg4Type))
	{
		std::lock_guard<ThreadMode> lock(m_slotListMutex);

		Bool needCreate = True;
		for (auto& conn : m_slotList)
		{
			const auto slotId = conn->GetSlotId();
			if (slotId == object->GetSlotId(this))
			{
				auto connPtr = reinterpret_cast<ClassConnection4<ClassType, Arg1Type, Arg2Type, Arg3Type, Arg4Type>*>(conn.get());
				if (connPtr)
				{
					needCreate = False;
					connPtr->AddCallableObject(memberFunction);
					break;
				}
			}
		}

		if (needCreate)
		{
			auto conn = std::unique_ptr<SlotConnection>(
				new ClassConnection4<ClassType, Arg1Type, Arg2Type, Arg3Type, Arg4Type>(m_lastId, object, memberFunction));
			object->SignalConnected(this, m_lastId, m_existMutex);
			m_slotList.push_back(std::move(conn));
			m_lastId++;
		}
	}

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type, class ThreadMode>
	typename Signal4<Arg1Type, Arg2Type, Arg3Type, Arg4Type, ThreadMode>::TrackAnonymousPtr 
	Signal4<Arg1Type, Arg2Type, Arg3Type, Arg4Type, ThreadMode>::Connect(AnonymousHandler function)
	{
		std::lock_guard<ThreadMode> lock(m_slotListMutex);

		auto track = std::unique_ptr<TrackAnonymous>(new TrackAnonymous(this, m_lastId, m_existMutex));
		auto conn = std::unique_ptr<SlotConnection>(new AnonymousConnection(m_lastId, function));
		m_slotList.push_back(std::move(conn));
		m_lastId++;
		
		return std::move(track);
	}

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type, class ThreadMode>
	Void Signal4<Arg1Type, Arg2Type, Arg3Type, Arg4Type, ThreadMode>::Disconnect(SlotId id)
	{
		std::lock_guard<ThreadMode> lock(m_slotListMutex);

		auto newEnd = std::remove_if(std::begin(m_slotList), std::end(m_slotList), [id](const std::unique_ptr<SlotConnection>& conn)
		{
			return conn->GetSlotId() == id;
		});
		m_slotList.erase(newEnd, std::end(m_slotList));
	}

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type, class ThreadMode>
	Void Signal4<Arg1Type, Arg2Type, Arg3Type, Arg4Type, ThreadMode>::DisconnectAll()
	{
		std::lock_guard<ThreadMode> lock(m_slotListMutex);
		m_slotList.clear();
	}

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type, class ThreadMode>
	Void Signal4<Arg1Type, Arg2Type, Arg3Type, Arg4Type, ThreadMode>::Emit(Arg1Type arg1, Arg2Type arg2, Arg3Type arg3, Arg4Type arg4)
	{
		std::lock_guard<ThreadMode> lock(m_slotListMutex);

		for (auto& conn : m_slotList)
		{
			conn->Emit(arg1, arg2, arg3, arg4);
		}
	}

	template <typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type, class ThreadMode>
	Void Signal4<Arg1Type, Arg2Type, Arg3Type, Arg4Type, ThreadMode>::operator()(Arg1Type arg1, Arg2Type arg2, Arg3Type arg3, Arg4Type arg4)
	{
		Emit(arg1, arg2, arg3, arg4);
	}
} }