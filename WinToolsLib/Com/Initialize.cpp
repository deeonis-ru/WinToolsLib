#include "Initialize.h"

#include <Objbase.h>
#pragma comment(lib, "Ole32.lib")

namespace WinToolsLib
{
	namespace Com
	{
		Initialize::Initialize(Model model) :
			m_successful(False)
		{
			HRESULT hr = E_NOTIMPL;
			switch (model)
			{
			case Model::SingleThreaded:
				hr = ::CoInitialize(nullptr);
				break;
				
			case Model::ApartmentThreaded:
				hr = ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
				break;
				
			case Model::Multithreaded:
				hr = ::CoInitializeEx(nullptr, COINIT_MULTITHREADED);
				break;
			}
			if (S_OK == hr || S_FALSE == hr)
			{
				m_successful = True;
			}
		}

		Initialize::~Initialize()
		{
			if (m_successful)
			{
				::CoUninitialize();
			}
		}
	}
}