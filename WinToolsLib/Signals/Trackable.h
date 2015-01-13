#pragma once
#include "..\Types.h"

#include "MultiThreadModes.h"
#include "SignalBase.h"

#include <memory>
#include <vector>
#include <tuple>

namespace WinToolsLib { namespace Signals
{
	class Track
	{
		Track(const Track& other);
		Track& operator=(const Track& other);

		//Track(Track&& other);
		//Track& operator=(Track&& other);

	public:
		Track() {};

		virtual Void Disable() = 0;
		virtual SlotId GetSlotId(const SignalBase* signal) const = 0;
	};

	template <class ThreadMode = OSHI_SIGNALS_DEFAULT_THREAD_MODE>
	class ClassTrack : public Track
	{
	public:
		typedef std::shared_ptr<ThreadMode> SignalExistMutex;
		typedef std::shared_ptr<ThreadMode> SignalMutexPtr;
		typedef std::tuple<SignalBase*, SlotId, SignalMutexPtr, SignalExistFlag> SignalListItem;

		ClassTrack();
		~ClassTrack();

		//ClassTrack(ClassTrack&& other);
		//ClassTrack& operator=(ClassTrack&& other);

		Void Disable() override;
		SlotId GetSlotId(const SignalBase* signal) const override;
		Void SignalConnected(SignalBase* signal, SlotId id, const SignalExistMutex& mutex);

	//private:
	//	Void MoveFrom(ClassTrack& other);

	private:
		std::vector<SignalListItem> m_signalList;
	};

	template <class ThreadMode>
	ClassTrack<ThreadMode>::ClassTrack()
	{
	}

	template <class ThreadMode>
	ClassTrack<ThreadMode>::~ClassTrack()
	{
		Disable();
	}

	//template <class ThreadMode>
	//ClassTrack<ThreadMode>::ClassTrack(ClassTrack<ThreadMode>&& other)
	//{
	//	MoveFrom(other);
	//}

	//template <class ThreadMode>
	//ClassTrack<ThreadMode>& ClassTrack<ThreadMode>::operator=(ClassTrack<ThreadMode>&& other)
	//{
	//	MoveFrom(other);
	//	return *this;
	//}

	template <class ThreadMode>
	Void ClassTrack<ThreadMode>::Disable()
	{
		for (auto& signalTuple : m_signalList)
		{
			auto& mutex = std::get<2>(signalTuple);
			std::lock_guard<ThreadMode> lock(*mutex);

			auto& flag = std::get<3>(signalTuple);
			if (*flag)
			{
				auto signal = std::get<0>(signalTuple);
				auto slotId = std::get<1>(signalTuple);
				signal->Disconnect(slotId);
			}
		}

		m_signalList.clear();
	}

	template <class ThreadMode>
	SlotId ClassTrack<ThreadMode>::GetSlotId(const SignalBase* signal) const
	{
		const auto it = std::find_if(std::begin(m_signalList), std::end(m_signalList), [signal](const SignalListItem& item)
		{
			const auto storedSignal = std::get<0>(item);
			return storedSignal == signal;
		});

		if (it != std::end(m_signalList))
		{
			auto slotId = std::get<1>(*it);
			return slotId;
		}
		else
		{
			return ULLONG_MAX;
		}
	}

	template <class ThreadMode>
	Void ClassTrack<ThreadMode>::SignalConnected(SignalBase* signal, SlotId id, const SignalExistMutex& mutex)
	{
		m_signalList.push_back(std::make_tuple(signal, id, mutex, signal->GetExistFlag()));
	}

	//template <class ThreadMode>
	//Void ClassTrack<ThreadMode>::MoveFrom(ClassTrack<ThreadMode>& other)
	//{
	//	if (this != &other)
	//	{
	//		m_signalList = std::move(other.m_signalList);
	//	}
	//}

	template <class ThreadMode>
	class AnonymousTrack : public Track
	{
	public:
		typedef std::shared_ptr<ThreadMode> SignalExistMutex;

		AnonymousTrack();
		AnonymousTrack(SignalBase* signal, SlotId id, const SignalExistMutex& mutex);
		~AnonymousTrack();

		AnonymousTrack(AnonymousTrack&& other);
		AnonymousTrack& operator=(AnonymousTrack&& other);

		Void Disable() override;
		SlotId GetSlotId(const SignalBase* signal) const override;

	private:
		Void MoveFrom(AnonymousTrack& other);

	private:
		SignalBase* m_signal;
		SignalExistFlag m_flag;
		SignalExistMutex m_mutex;
		SlotId m_slotId;
	};

	template <class ThreadMode>
	AnonymousTrack<ThreadMode>::AnonymousTrack() :
		m_signal(nullptr),
		m_slotId(ULLONG_MAX)
	{
	}

	template <class ThreadMode>
	AnonymousTrack<ThreadMode>::AnonymousTrack(SignalBase* signal, SlotId id, const SignalExistMutex& mutex) :
		m_signal(signal),
		m_flag(signal->GetExistFlag()),
		m_mutex(mutex),
		m_slotId(id)
	{
	}

	template <class ThreadMode>
	AnonymousTrack<ThreadMode>::~AnonymousTrack()
	{
		Disable();
	}

	template <class ThreadMode>
	AnonymousTrack<ThreadMode>::AnonymousTrack(AnonymousTrack<ThreadMode>&& other)
	{
		MoveFrom(other);
	}

	template <class ThreadMode>
	AnonymousTrack<ThreadMode>& AnonymousTrack<ThreadMode>::operator=(AnonymousTrack<ThreadMode>&& other)
	{
		MoveFrom(other);
		return *this;
	}

	template <class ThreadMode>
	Void AnonymousTrack<ThreadMode>::Disable()
	{
		if (!m_signal)
			return;

		std::lock_guard<ThreadMode> lock(*m_mutex);
		if (*m_flag)
		{
			m_signal->Disconnect(m_slotId);
		}
		m_signal = nullptr;
		m_slotId = ULLONG_MAX;
	}

	template <class ThreadMode>
	SlotId AnonymousTrack<ThreadMode>::GetSlotId(const SignalBase* signal) const
	{
		if (signal == m_signal)
			return m_slotId;
		else
			return ULLONG_MAX;
	}

	template <class ThreadMode>
	Void AnonymousTrack<ThreadMode>::MoveFrom(AnonymousTrack<ThreadMode>& other)
	{
		if (this != &other)
		{
			m_flag = std::move(other.m_flag);
			m_mutex = std::move(other.m_mutex);
			m_signal = other.m_signal;
			m_slotId = other.m_slotId;

			other.m_signal = nullptr;
			other.m_slotId = ULLONG_MAX;
		}
	}
} } 