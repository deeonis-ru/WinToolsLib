#pragma once
#include "..\..\Types.h"
#include "..\..\Com\Ptr.h"

#include <functional>

#include <Windows.h>
#include <WbemIdl.h>
#pragma comment(lib, "Wbemuuid.lib")

#include "WbemClassObject.h"

namespace WinToolsLib { namespace Os { namespace Wmi
{
	class WbemServices
	{
		WbemServices(const WbemServices& other);            // Non-copyable
		WbemServices& operator=(const WbemServices& other); // Non-copyable

	public:
		WbemServices();
		~WbemServices();

		WbemServices(WbemServices&& other);
		WbemServices& operator=(WbemServices&& other);

		WbemClassObject GetObject(const TChar* objectPath);
		
		WbemClassObject ExecMethod(
			const TChar* objectPath,
			const TChar* methodName,
			const WbemClassObject& inParams);
		
		typedef std::function<Bool(WbemClassObject object)> EnumCallback;
		Void ExecQuery(const TChar* query, EnumCallback callback);

	protected:
		Void MoveFrom(WbemServices& other);

	private:
		Com::Ptr<IWbemServices> m_services;
	};

} } }