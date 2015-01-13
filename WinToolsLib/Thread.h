#pragma once
#include "Types.h"
#include "Exception.h"
#include "IWaitable.h"
#include "Handles\Handle.h"
#include <memory>
#include <list>

namespace WinToolsLib
{
	class Thread;
	typedef std::list<Thread> ThreadList;

	using namespace Handles;

	class Thread : public IWaitable
	{
	public:
		explicit Thread(UInt32 id);
		Thread(UInt32 id, Handle&& handle);
		Thread(UInt32 id, const Handle& handle);
		Thread(Handle&& handle);
		Thread(const Handle& handle);
		Thread(Thread&& other);
		Thread(const Thread& other);

		Thread& operator=(Thread&& other);
		Thread& operator=(const Thread& other);

		Void Wait() const override;
		Void Wait(UInt32 milliseconds) const override;
		Void Suspend();
		Void Resume();

		template <class Callback>
		static Thread Create(Callback callback, Bool suspended = False);

		static Void Sleep(UInt32 milliseconds);

		static ThreadList GetList(UInt32 processId);
	
	protected:
		Void MoveFrom(Thread& other);
		Void CopyFrom(const Thread& other);

	private:
		UInt32 m_id;
		Handle m_handle;

		static const UInt32 m_invalidId = -1;
	};

	template <class Callback>
	Thread Thread::Create(Callback callback, Bool suspended)
	{
		struct ThreadImpl
		{
			static DWORD WINAPI EntryPoint(LPVOID pParam)
			{
				StructuredException::EnableInThisThread();
				std::unique_ptr<Callback> callbackPtr((Callback*)pParam);
				(*callbackPtr)();
				return 0;
			}
		};

		Handle handle = ::CreateThread(
			NULL,
			0,
			ThreadImpl::EntryPoint,
			new Callback(callback),
			(suspended) ? CREATE_SUSPENDED : 0,
			NULL);

		if (handle.IsNull())
		{
			THROW_LAST_ERROR_EXCEPTION();
		}

		return Thread(std::move(handle));
	}
}
