#pragma once
#include "..\Types.h"
#include <Unknwn.h>
#include <type_traits>

namespace WinToolsLib
{
	namespace Com
	{
		namespace Details
		{
			template <class T>
			class NoAddRefOrRelease : public T
			{
			private:
				ULONG STDMETHODCALLTYPE AddRef() override = 0;
				ULONG STDMETHODCALLTYPE Release() override = 0;
			};
		}

		template <class T>
		class Ptr
		{
			static_assert(std::is_base_of<IUnknown, T>::value, "T must be derived from IUnknown");

			Ptr(const Ptr& other);            // Non-copyable
			Ptr& operator=(const Ptr& other); // Non-copyable

		public:
			Ptr(T* p = nullptr);
			Ptr(Ptr&& other);
			~Ptr();

			Ptr& operator=(Ptr&& other);

			Void Attach(T* p); // Just store IUnknown without calling AddRef()
			T* Detach();       // Detach IUnknown so Release() wouldn't be called
			Void Release();

			operator T*() const;
			T& operator *() const;
			T** operator &();

			Details::NoAddRefOrRelease<T>* operator ->() const;

		protected:
			Void MoveFrom(Ptr& other);

		private:
			T* m_p;
		};

		template <class T>
		Ptr<T>::Ptr(T* p) :
			m_p(p)
		{
			if (m_p)
			{
				m_p->AddRef();
			}
		}

		template <class T>
		Ptr<T>::Ptr(Ptr && other) :
			m_p(nullptr)
		{
			MoveFrom(other);
		}

		template <class T>
		Ptr<T>::~Ptr()
		{
			if (m_p)
			{
				m_p->Release();
			}
		}
		
		template <class T>
		Ptr<T>& Ptr<T>::operator=(Ptr&& other)
		{
			MoveFrom(other);
			return *this;
		}

		template <class T>
		Void Ptr<T>::Attach(T* p)
		{
			Release();
			m_p = p;
		}
		
		template <class T>
		T* Ptr<T>::Detach()
		{
			T* p = m_p;
			m_p = nullptr;
			return p;
		}

		template <class T>
		Void Ptr<T>::Release()
		{
			T* p = m_p;
			if (p)
			{
				m_p = nullptr;
				p->Release();
			}
		}

		template <class T>
		Ptr<T>::operator T*() const
		{
			return m_p;
		}

		template <class T>
		T& Ptr<T>::operator *() const
		{
			return *m_p;
		}

		template <class T>
		T** Ptr<T>::operator &()
		{
			return &m_p;
		}
		
		template <class T>
		Details::NoAddRefOrRelease<T>* Ptr<T>::operator ->() const
		{
			return (Details::NoAddRefOrRelease<T>*)m_p;
		}

		template <class T>
		Void Ptr<T>::MoveFrom(Ptr& other)
		{
			if (&m_p != &other.m_p)
			{
				Release();
				m_p = other.m_p;
				other.m_p = nullptr;
			}
		}
	}
}
