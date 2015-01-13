#include "WbemClassObject.h"
#include "..\..\Exception.h"

namespace WinToolsLib { namespace Os { namespace Wmi
{
	WbemClassObject::WbemClassObject(Com::Ptr<IWbemClassObject> object) :
		m_object(std::move(object))
	{
	}

	WbemClassObject::~WbemClassObject()
	{
	}

	WbemClassObject::WbemClassObject(WbemClassObject&& other)
	{
		MoveFrom(other);
	}

	WbemClassObject& WbemClassObject::operator=(WbemClassObject&& other)
	{
		MoveFrom(other);
		return *this;
	}

	Com::Variant WbemClassObject::Get(const WChar* name)
	{
		Com::Variant value;

		auto hr = m_object->Get(name, 0, &value, nullptr, nullptr);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return value;
	}

	Void WbemClassObject::Set(const WChar* name, Com::Variant& value)
	{
		auto hr = m_object->Put(name, 0, &value, 0);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	WbemClassObject WbemClassObject::GetMethodInSignature(const WChar* name)
	{
		Com::Ptr<IWbemClassObject> inSignature;

		auto hr = m_object->GetMethod(name, 0, &inSignature, nullptr);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return WbemClassObject(std::move(inSignature));
	}

	WbemClassObject WbemClassObject::SpawnInstance()
	{
		Com::Ptr<IWbemClassObject> instance;

		auto hr = m_object->SpawnInstance(0, &instance);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return WbemClassObject(std::move(instance));
	}

	Void WbemClassObject::MoveFrom(WbemClassObject& other)
	{
		if (this != &other)
		{
			m_object = std::move(other.m_object);
		}
	}

} } }