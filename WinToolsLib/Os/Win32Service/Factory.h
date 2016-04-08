#pragma once
#include <Windows.h>

#include <vector>

#include "..\..\Types.h"
#include "Service.h"

namespace WinToolsLib { namespace Os { namespace Win32Service
{
	class Factory
	{
	public:
		template <class Instance>
		static Void RegisterService();

		static UInt32 RunRegisteredServices();

	private:
		static std::vector<SERVICE_TABLE_ENTRY> m_serviceList;
	};

	template <class Instance>
	Void Factory::RegisterService()
	{
		SERVICE_TABLE_ENTRY entry = { Instance::GetName(), ServiceT<Instance>::Main };
		m_serviceList.push_back(entry);
	}
}}}