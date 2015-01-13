#pragma once
#include "..\Types.h"
#include <Windows.h>

namespace WinToolsLib
{
	namespace Com
	{
		template <class T>
		class TaskMem
		{
			TaskMem(const TaskMem& other);            // Non-copyable
			TaskMem& operator=(const TaskMem& other); // Non-copyable

		public:
			TaskMem(T* p = nullptr);
			TaskMem(TaskMem&& other);
			~TaskMem();

			TaskMem& operator=(TaskMem&& other);

			Void Free();
			static TaskMem Alloc(SizeT bytes);
			
			operator T*() const;
			T& operator *() const;
			T* operator ->() const;
			T** operator &();

		private:
			T* m_p;
		};

		namespace Details
		{
			PVoid Alloc(SizeT bytes);
			Void Free(PVoid p);
		}

		template <class T>
		TaskMem<T>::TaskMem(T* p) :
			m_p(p)
		{
		}

		template <class T>
		TaskMem<T>::TaskMem(TaskMem<T>&& other) :
			m_p(other.m_p)
		{
			other.m_p = nullptr;
		}

		template <class T>
		TaskMem<T>::~TaskMem()
		{
			Details::Free(m_p);	
		}

		template <class T>
		TaskMem<T>& TaskMem<T>::operator=(TaskMem<T>&& other)
		{
			Free();
			m_p = other.m_p;
			other.m_p = nullptr;
			return *this;
		}

		template <class T>
		Void TaskMem<T>::Free()
		{
			T* p = m_p;
			if (p)
			{
				m_p = nullptr;
				Details::Free(p);	
			}
		}

		template <class T>
		TaskMem<T> TaskMem<T>::Alloc(SizeT bytes)
		{
			return TaskMem((T*)Details::Alloc(bytes));
		}

		template <class T>
		TaskMem<T>::operator T*() const
		{
			return m_p;
		}

		template <class T>
		T& TaskMem<T>::operator *() const
		{
			return *m_p;
		}

		template <class T>
		T* TaskMem<T>::operator ->() const
		{
			return m_p;
		}

		template <class T>
		T** TaskMem<T>::operator &()
		{
			return &m_p;
		}
	}
}

