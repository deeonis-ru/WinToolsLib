#include "Service.h"

#include "..\..\Exception.h"

namespace WinToolsLib { namespace Os { namespace Win32Service { namespace Details
{
	Void ReportEvent(PCTChar serviceName, PCTChar function, UInt32 error)
	{
		auto eventSource = ::RegisterEventSource(NULL, serviceName);

		if (NULL != eventSource)
		{
			auto msg = String::FormatS(Text("%s failed with %d"), function, error);

			PCTChar strings[2];
			strings[0] = serviceName;
			strings[1] = msg.GetBuffer();

			::ReportEvent(eventSource,        // event log handle
				EVENTLOG_ERROR_TYPE, // event type
				0,                   // event category
				0,                   // event identifier
				NULL,                // no security identifier
				2,                   // size of lpszStrings array
				0,                   // no binary data
				strings,             // array of strings
				NULL);               // no binary data

			::DeregisterEventSource(eventSource);
		}
	}
}}}}