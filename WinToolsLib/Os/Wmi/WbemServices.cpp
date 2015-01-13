#include "WbemServices.h"
#include "..\..\Exception.h"
#include "..\..\Trace.h"

#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")

namespace WinToolsLib { namespace Os { namespace Wmi
{
	WbemServices::WbemServices()
	{
		auto hr = ::CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, 0);
		if (FAILED(hr) && RPC_E_TOO_LATE != hr)
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		Com::Ptr<IWbemLocator> pLocator;

		hr = ::CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pLocator));
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		hr = pLocator->ConnectServer(L"root\\cimv2", NULL, NULL, NULL, WBEM_FLAG_CONNECT_USE_MAX_WAIT, NULL, NULL, &m_services);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		hr = ::CoSetProxyBlanket(m_services, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	WbemServices::~WbemServices()
	{
	}

	WbemServices::WbemServices(WbemServices&& other)
	{
		MoveFrom(other);
	}

	WbemServices& WbemServices::operator=(WbemServices&& other)
	{
		MoveFrom(other);
		return *this;
	}

	WbemClassObject WbemServices::GetObject(const TChar* objectPath)
	{
		Com::Ptr<IWbemClassObject> object;

		auto hr = m_services->GetObject(_bstr_t(objectPath), 0, nullptr, &object, nullptr);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return WbemClassObject(std::move(object));
	}

	WbemClassObject WbemServices::ExecMethod(
		const TChar* objectPath,
		const TChar* methodName,
		const WbemClassObject& inParams)
	{
		Com::Ptr<IWbemClassObject> outParams;

		auto hr = m_services->ExecMethod(_bstr_t(objectPath), _bstr_t(methodName), 0, nullptr, inParams, &outParams, nullptr);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return WbemClassObject(std::move(outParams));
	}

	Void WbemServices::ExecQuery(const TChar* query, EnumCallback callback)
	{
		Com::Ptr<IEnumWbemClassObject> enumerator;

		auto hr = m_services->ExecQuery(L"WQL", _bstr_t(query), WBEM_FLAG_FORWARD_ONLY, nullptr, &enumerator);
		if (FAILED(hr))
		{
			StringW wql(query);
			TRACE_ERROR(L"Exception while executing WQL query: %s", wql.GetBuffer());
			THROW_WIN32_EXCEPTION(hr);
		}

		while (nullptr != enumerator)
		{
			ULONG returned = 0;
			Com::Ptr<IWbemClassObject> object;

			auto hr = enumerator->Next(WBEM_INFINITE, 1, &object, &returned);
			if (0 == returned || nullptr == object)
			{
				break;
			}

			auto next = callback(WbemClassObject(std::move(object)));
			if (!next)
			{
				break;
			}
		}
	}

	Void WbemServices::MoveFrom(WbemServices& other)
	{
		if (this != &other)
		{
			m_services = std::move(other.m_services);
		}
	}

} } }