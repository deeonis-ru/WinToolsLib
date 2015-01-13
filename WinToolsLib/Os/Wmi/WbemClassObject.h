#pragma once
#include "..\..\Types.h"
#include "..\..\Com\Ptr.h"
#include "..\..\Com\Variant.h"

#include <Windows.h>
#include <WbemIdl.h>
#pragma comment(lib, "Wbemuuid.lib")

namespace WinToolsLib { namespace Os { namespace Wmi
{
	class WbemClassObject
	{
		WbemClassObject(const WbemClassObject& other);            // Non-copyable
		WbemClassObject& operator=(const WbemClassObject& other); // Non-copyable

	public:
		WbemClassObject(Com::Ptr<IWbemClassObject> object);
		~WbemClassObject();

		WbemClassObject(WbemClassObject&& other);
		WbemClassObject& operator=(WbemClassObject&& other);

		operator IWbemClassObject*() const;

		Com::Variant Get(const WChar* name);
		Void Set(const WChar* name, Com::Variant& value);
		
		WbemClassObject GetMethodInSignature(const WChar* name);
		WbemClassObject SpawnInstance();

	protected:
		Void MoveFrom(WbemClassObject& other);

	private:
		Com::Ptr<IWbemClassObject> m_object;
	};

	inline WbemClassObject::operator IWbemClassObject*() const
	{
		return m_object;
	}

} } }