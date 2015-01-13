#pragma once
#include "..\Types.h"
#include <functional>

namespace WinToolsLib
{
	namespace Handles
	{
		template <class HandleType>
		class HandleBase
		{
		public:
			typedef std::function<Void(HandleType)> Finalizer;

			HandleBase(Finalizer finalizer);
			HandleBase(Finalizer finalizer, HandleType handle);
			~HandleBase();

			Bool IsNull() const;

			Void Close();
			Void Reset(HandleType handle);

			HandleType Detach();
			operator HandleType() const;

		protected:
			Finalizer  m_finalizer;
			HandleType m_handle;
		};

		template <class HandleType>
		HandleBase<HandleType>::HandleBase(Finalizer finalizer) :
			m_finalizer(finalizer),
			m_handle(NULL)
		{
		}

		template <class HandleType>
		HandleBase<HandleType>::HandleBase(Finalizer finalizer, HandleType handle) :
			m_finalizer(finalizer),
			m_handle(handle)
		{
		}

		template <class HandleType>
		HandleBase<HandleType>::~HandleBase()
		{
			Close();
		}

		template <class HandleType>
		Bool HandleBase<HandleType>::IsNull() const
		{
			if (NULL == m_handle)
				return True;

			return False;
		}

		template <class HandleType>
		Void HandleBase<HandleType>::Close()
		{
			if (!IsNull())
			{
				m_finalizer(m_handle);
				m_handle = NULL;
			}
		}

		template <class HandleType>
		Void HandleBase<HandleType>::Reset(HandleType handle)
		{
			Close();
			m_handle = handle;
		}

		template <class HandleType>
		HandleType HandleBase<HandleType>::Detach()
		{
			HandleType handle = m_handle;
			m_handle = NULL;
			return handle;
		}

		template <class HandleType>
		HandleBase<HandleType>::operator HandleType() const
		{
			return m_handle;
		}
	}
}