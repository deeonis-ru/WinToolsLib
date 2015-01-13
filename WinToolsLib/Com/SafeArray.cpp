#include "SafeArray.h"
#include "..\Exception.h"

#include <OleAuto.h>
#pragma comment(lib, "OleAut32.lib")

#include <type_traits>

namespace WinToolsLib { namespace Com
{
	SafeArray::SafeArray(SAFEARRAY* safeArray) :
		m_safeArray(safeArray)
	{
	}

	SafeArray::SafeArray(SafeArray&& other) :
		m_safeArray(nullptr)
	{
		MoveFrom(other);
	}

	SafeArray::SafeArray(const SafeArray& other) :
		m_safeArray(nullptr)
	{
		CopyFrom(other);
	}

	SafeArray::~SafeArray()
	{
		Destroy();
	}

	SafeArray SafeArray::Create(VARTYPE vt, UInt32 size)
	{
		return SafeArray(::SafeArrayCreateVector(vt, 0, size));
	}

	Void SafeArray::Destroy()
	{
		::SafeArrayDestroy(Detach());
	}

	Void SafeArray::Attach(SAFEARRAY* safeArray)
	{
		Destroy();
		m_safeArray = safeArray;
	}

	SAFEARRAY* SafeArray::Detach()
	{
		auto safeArray = m_safeArray;
		m_safeArray = nullptr;
		return safeArray;
	}

	UInt32 SafeArray::GetSize() const
	{
		if (nullptr == m_safeArray)
		{
			return 0;
		}

		LONG lbound = 0;

		auto hr = ::SafeArrayGetLBound(m_safeArray, 1, &lbound);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
		
		LONG ubound = 0;

		hr = ::SafeArrayGetUBound(m_safeArray, 1, &ubound);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return static_cast<UInt32>(ubound - lbound + 1);
	}

	Void SafeArray::GetElement(Int32 index, PVoid value)
	{
		auto hr = ::SafeArrayGetElement(m_safeArray, (LONG*)&index, value);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Void SafeArray::SetElement(Int32 index, PVoid value)
	{
		auto hr = ::SafeArrayPutElement(m_safeArray, (LONG*)&index, value);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	SafeArray& SafeArray::operator=(SafeArray&& other)
	{
		MoveFrom(other);
		return *this;
	}

	SafeArray& SafeArray::operator=(const SafeArray& other)
	{
		CopyFrom(other);
		return *this;
	}

	Void SafeArray::MoveFrom(SafeArray& other)
	{
		if (this != &other)
		{
			Attach(other.Detach());
		}
	}

	Void SafeArray::CopyFrom(const SafeArray& other)
	{
		if (this != &other)
		{
			Destroy();

			auto hr = ::SafeArrayCopy(other.m_safeArray, &m_safeArray);
			if (FAILED(hr))
			{
				THROW_WIN32_EXCEPTION(hr);
			}
		}
	}

} }