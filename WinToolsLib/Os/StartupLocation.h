#pragma once
#include "..\Types.h"
#include "..\Exception.h"

namespace WinToolsLib { namespace Os
{
	class Startup
	{
	public:
		enum class FileLocation
		{
			Unknown = 0,
			UserStartup,
			CommonStartup
		};
		enum class RegistryLocation
		{
			Unknown = 0,
			CurrentUserRun,
			CurrentUserRunOnce,
			LocalMachineRun,
			LocalMachineRun32,
			LocalMachineRunOnce,
			LocalMachineRunOnce32
		};
	};

	class UnsupportedLocation : public Exception
	{
	public:
		UnsupportedLocation(PCChar function, UInt32 line, PCChar file);
	};

	inline UnsupportedLocation::UnsupportedLocation(PCChar function, UInt32 line, PCChar file) :
		Exception(function, line, file, Text("Unsupported location"))
	{
	}

	#define THROW_UNSUPPORTED_LOCATION() \
		throw UnsupportedLocation(__FUNCTION__, __LINE__, __FILE__)

} }