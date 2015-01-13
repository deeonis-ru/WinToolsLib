#pragma once
#include "..\Types.h"
#include "..\String.h"

#include <Windows.h>

namespace WinToolsLib { namespace Com
{
	class SafeArray
	{
	public:
		SafeArray(SAFEARRAY* safeArray);
		SafeArray(SafeArray&& other);
		SafeArray(const SafeArray& other);
		~SafeArray();

		static SafeArray Create(VARTYPE vt, UInt32 size);
		Void Destroy();

		Void Attach(SAFEARRAY* safeArray);
		SAFEARRAY* Detach();

		UInt32 GetSize() const;
		operator const SAFEARRAY*() const;

		Void GetElement(Int32 index, PVoid value);
		Void SetElement(Int32 index, PVoid value);

		SafeArray& operator=(SafeArray&& other);
		SafeArray& operator=(const SafeArray& other);

	protected:
		Void MoveFrom(SafeArray& other);
		Void CopyFrom(const SafeArray& other);

	private:
		SAFEARRAY* m_safeArray;
	};

	inline SafeArray::operator const SAFEARRAY*() const
	{
		return m_safeArray;
	}

} }

